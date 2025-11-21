const express = require('express');
const cors = require('cors');
const bodyParser = require('body-parser');
const { spawn } = require('child_process');
const path = require('path');

const app = express();
const PORT = process.env.PORT || 3001;

app.use(cors());
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

const CPP_EXECUTABLE = path.join(__dirname, '..', 'cpp_backend', 'lms.exe');

// No auth/session – reverted to open endpoints

function executeCppCommand(command, args = []) {
    return new Promise((resolve, reject) => {
        const fullCommand = [command, ...args].join(' ');
        console.log(`Executing C++ command: ${fullCommand}`);
        
        const cppProcess = spawn(CPP_EXECUTABLE, [command, ...args], {
            cwd: path.join(__dirname, '..', 'cpp_backend')
        });

        let stdout = '';
        let stderr = '';

        cppProcess.stdout.on('data', (data) => {
            stdout += data.toString();
        });

        cppProcess.stderr.on('data', (data) => {
            stderr += data.toString();
        });

        cppProcess.on('close', (code) => {
            if (code !== 0) {
                console.error(`C++ process exited with code ${code}`);
                console.error(`stderr: ${stderr}`);
                reject(new Error(`C++ execution failed: ${stderr}`));
            } else {
                try {
                    // Try to parse as JSON first
                    const result = JSON.parse(stdout);
                    resolve(result);
                } catch (e) {
                    // If it's not JSON, check if it contains JSON data in the message
                    const message = stdout.trim();
                    
                    // Look for JSON array or object in the output
                    const jsonMatch = message.match(/\[\s*\{.*\}\s*\]|\{\s*.*\}\s*}/s);
                    if (jsonMatch) {
                        try {
                            const jsonData = JSON.parse(jsonMatch[0]);
                            resolve({ success: true, data: jsonData });
                        } catch (jsonError) {
                            resolve({ success: true, message: message });
                        }
                    } else {
                        resolve({ success: true, message: message });
                    }
                }
            }
        });

        cppProcess.on('error', (error) => {
            reject(new Error(`Failed to spawn C++ process: ${error.message}`));
        });
    });
}

// Login endpoints removed

app.get('/api/books', async (req, res) => {
    try {
        const result = await executeCppCommand('get_books_json');
        if (Array.isArray(result)) {
            res.json({ success: true, data: result });
        } else {
            res.json(result);
        }
    } catch (error) {
        res.status(500).json({ success: false, error: error.message });
    }
});

app.post('/api/books', async (req, res) => {
    try {
        const { isbn, title, author, publicationYear, copies } = req.body;
        const result = await executeCppCommand('add_book', [isbn, title, author, publicationYear.toString(), copies.toString()]);
        res.json(result);
    } catch (error) {
        res.status(500).json({ success: false, error: error.message });
    }
});

app.get('/api/books/search/:query', async (req, res) => {
    try {
        const result = await executeCppCommand('search_book_title', [req.params.query]);
        if (Array.isArray(result)) {
            res.json({ success: true, data: result });
        } else {
            res.json(result);
        }
    } catch (error) {
        res.status(500).json({ success: false, error: error.message });
    }
});

app.get('/api/books/:isbn', async (req, res) => {
    try {
        const result = await executeCppCommand('search_book_isbn', [req.params.isbn]);
        res.json(result);
    } catch (error) {
        res.status(500).json({ success: false, error: error.message });
    }
});

app.put('/api/books/:isbn', async (req, res) => {
    try {
        const { title, author, publicationYear, copies } = req.body;
        const result = await executeCppCommand('update_book', [req.params.isbn, title, author, publicationYear.toString(), copies.toString()]);
        res.json(result);
    } catch (error) {
        res.status(500).json({ success: false, error: error.message });
    }
});

app.delete('/api/books/:isbn', async (req, res) => {
    try {
        const result = await executeCppCommand('delete_book', [req.params.isbn]);
        const msg = (result && result.message) ? result.message : (typeof result === 'string' ? result : '');
        if (msg.toLowerCase().includes('deleted successfully')) {
            return res.json({ success: true, message: msg });
        }
        return res.status(400).json({ success: false, error: msg || 'Delete failed' });
    } catch (error) {
        res.status(500).json({ success: false, error: error.message });
    }
});

app.get('/api/users', async (req, res) => {
    try {
        const result = await executeCppCommand('get_users_json');
        if (Array.isArray(result)) {
            res.json({ success: true, data: result });
        } else {
            res.json(result);
        }
    } catch (error) {
        res.status(500).json({ success: false, error: error.message });
    }
});

app.post('/api/users', async (req, res) => {
    try {
        const { userId, name, email, phone } = req.body;
        const result = await executeCppCommand('add_user', [userId, name, email, phone]);
        res.json(result);
    } catch (error) {
        res.status(500).json({ success: false, error: error.message });
    }
});

app.get('/api/users/search/:query', async (req, res) => {
    try {
        const result = await executeCppCommand('search_user_name', [req.params.query]);
        if (Array.isArray(result)) {
            res.json({ success: true, data: result });
        } else {
            res.json(result);
        }
    } catch (error) {
        res.status(500).json({ success: false, error: error.message });
    }
});

app.get('/api/users/:userId', async (req, res) => {
    try {
        const result = await executeCppCommand('search_user', [req.params.userId]);
        res.json(result);
    } catch (error) {
        res.status(500).json({ success: false, error: error.message });
    }
});

app.put('/api/users/:userId', async (req, res) => {
    try {
        const { name, email, phone } = req.body;
        const result = await executeCppCommand('update_user', [req.params.userId, name, email, phone]);
        res.json(result);
    } catch (error) {
        res.status(500).json({ success: false, error: error.message });
    }
});

app.delete('/api/users/:userId', async (req, res) => {
    try {
        const result = await executeCppCommand('delete_user', [req.params.userId]);
        const msg = (result && result.message) ? result.message : (typeof result === 'string' ? result : '');
        if (msg.toLowerCase().includes('deleted successfully')) {
            return res.json({ success: true, message: msg });
        }
        return res.status(400).json({ success: false, error: msg || 'Delete failed' });
    } catch (error) {
        res.status(500).json({ success: false, error: error.message });
    }
});

app.post('/api/transactions/borrow', async (req, res) => {
    try {
        const { userId, bookISBN } = req.body;
        const usersRes = await executeCppCommand('get_users_json');
        const users = usersRes.data || usersRes.users || [];
        const user = Array.isArray(users) ? users.find(u => u.userId === userId) : null;
        if (!user) {
            return res.status(404).json({ success: false, error: 'User not found' });
        }
        const booksRes = await executeCppCommand('get_books_json');
        const books = booksRes.data || booksRes.books || [];
        const book = Array.isArray(books) ? books.find(b => b.isbn === bookISBN) : null;
        if (!book) {
            return res.status(404).json({ success: false, error: 'Book not found' });
        }
        const result = await executeCppCommand('borrow_book', [userId, bookISBN]);
        const msg = (result && result.message) ? result.message : '';
        if (msg.toLowerCase().includes('successfully')) {
            return res.json(result);
        }
        return res.status(400).json({ success: false, error: msg || 'Borrow failed' });
    } catch (error) {
        res.status(500).json({ success: false, error: error.message });
    }
});

app.post('/api/transactions/return', async (req, res) => {
    try {
        const { userId, bookISBN } = req.body;
        const usersRes = await executeCppCommand('get_users_json');
        const users = usersRes.data || usersRes.users || [];
        const user = Array.isArray(users) ? users.find(u => u.userId === userId) : null;
        if (!user) {
            return res.status(404).json({ success: false, error: 'User not found' });
        }
        const booksRes = await executeCppCommand('get_books_json');
        const books = booksRes.data || booksRes.books || [];
        const book = Array.isArray(books) ? books.find(b => b.isbn === bookISBN) : null;
        if (!book) {
            return res.status(404).json({ success: false, error: 'Book not found' });
        }
        const result = await executeCppCommand('return_book', [userId, bookISBN]);
        const msg = (result && result.message) ? result.message : '';
        if (msg.toLowerCase().includes('successfully')) {
            return res.json(result);
        }
        return res.status(400).json({ success: false, error: msg || 'Return failed' });
    } catch (error) {
        res.status(500).json({ success: false, error: error.message });
    }
});

app.get('/api/transactions', async (req, res) => {
    try {
        const result = await executeCppCommand('get_transactions_json');
        if (Array.isArray(result)) {
            res.json({ success: true, data: result });
        } else {
            res.json(result);
        }
    } catch (error) {
        res.status(500).json({ success: false, error: error.message });
    }
});

app.get('/api/transactions/user/:userId', async (req, res) => {
    try {
        const result = await executeCppCommand('get_user_transactions', [req.params.userId]);
        res.json(result);
    } catch (error) {
        res.status(500).json({ success: false, error: error.message });
    }
});

app.get('/api/transactions/book/:bookISBN', async (req, res) => {
    try {
        const result = await executeCppCommand('get_book_transactions', [req.params.bookISBN]);
        res.json(result);
    } catch (error) {
        res.status(500).json({ success: false, error: error.message });
    }
});

app.get('/api/sort/books/:criteria', async (req, res) => {
    try {
        const criteria = (req.params.criteria || '').toLowerCase();
        const result = await executeCppCommand('get_books_json');
        const books = Array.isArray(result) ? result : (result.data || result.books || []);

        const comparators = {
            title: (a, b) => a.title.localeCompare(b.title),
            author: (a, b) => a.author.localeCompare(b.author),
            publicationyear: (a, b) => (a.publicationYear || 0) - (b.publicationYear || 0),
            isbn: (a, b) => a.isbn.localeCompare(b.isbn),
            availablecopies: (a, b) => (a.availableCopies || 0) - (b.availableCopies || 0),
            totalcopies: (a, b) => (a.totalCopies || 0) - (b.totalCopies || 0),
        };

        const cmp = comparators[criteria];
        if (!cmp) {
            return res.status(400).json({ success: false, error: `Unsupported sort criteria: ${criteria}` });
        }

        const sorted = [...books].sort(cmp);
        res.json({ success: true, data: sorted });
    } catch (error) {
        res.status(500).json({ success: false, error: error.message });
    }
});

app.get('/api/sort/users/:criteria', async (req, res) => {
    try {
        const criteria = (req.params.criteria || '').toLowerCase();
        const result = await executeCppCommand('get_users_json');
        const users = Array.isArray(result) ? result : (result.data || result.users || []);

        const comparators = {
            name: (a, b) => a.name.localeCompare(b.name),
            userid: (a, b) => a.userId.localeCompare(b.userId),
            email: (a, b) => a.email.localeCompare(b.email),
            phone: (a, b) => (a.phone || '').localeCompare(b.phone || ''),
        };

        const cmp = comparators[criteria];
        if (!cmp) {
            return res.status(400).json({ success: false, error: `Unsupported sort criteria: ${criteria}` });
        }

        const sorted = [...users].sort(cmp);
        res.json({ success: true, data: sorted });
    } catch (error) {
        res.status(500).json({ success: false, error: error.message });
    }
});

app.get('/api/sort/transactions/:criteria', async (req, res) => {
    try {
        const criteria = (req.params.criteria || '').toLowerCase();
        const result = await executeCppCommand('get_transactions_json');
        const txns = Array.isArray(result) ? result : (result.data || result.transactions || []);

        const comparators = {
            timestamp: (a, b) => new Date(a.timestamp).getTime() - new Date(b.timestamp).getTime(),
            userid: (a, b) => a.userId.localeCompare(b.userId),
            bookisbn: (a, b) => a.bookISBN.localeCompare(b.bookISBN),
            type: (a, b) => a.transactionType.localeCompare(b.transactionType),
        };

        const cmp = comparators[criteria];
        if (!cmp) {
            return res.status(400).json({ success: false, error: `Unsupported sort criteria: ${criteria}` });
        }

        const sorted = [...txns].sort(cmp);
        res.json({ success: true, data: sorted });
    } catch (error) {
        res.status(500).json({ success: false, error: error.message });
    }
});

app.use(express.static(path.join(__dirname, 'public')));

app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

app.listen(PORT, () => {
    console.log(`Server running on http://localhost:${PORT}`);
    console.log(`C++ executable path: ${CPP_EXECUTABLE}`);
});