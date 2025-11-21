const API_BASE = '/api';

let currentTab = 'books';

function showTab(tab) {
    currentTab = tab;
    
    // Update tab buttons
    document.getElementById('booksTab').className = 'px-4 py-2 bg-gray-200 text-gray-700 rounded-lg hover:bg-gray-300';
    document.getElementById('usersTab').className = 'px-4 py-2 bg-gray-200 text-gray-700 rounded-lg hover:bg-gray-300';
    document.getElementById('transactionsTab').className = 'px-4 py-2 bg-gray-200 text-gray-700 rounded-lg hover:bg-gray-300';
    
    document.getElementById(tab + 'Tab').className = 'px-4 py-2 bg-' + getTabColor(tab) + '-600 text-white rounded-lg';
    
    // Show/hide sections
    document.getElementById('booksSection').classList.add('hidden');
    document.getElementById('usersSection').classList.add('hidden');
    document.getElementById('transactionsSection').classList.add('hidden');
    
    document.getElementById(tab + 'Section').classList.remove('hidden');
    
    // Load data for the active tab
    loadData();
}

function getTabColor(tab) {
    switch(tab) {
        case 'books': return 'blue';
        case 'users': return 'green';
        case 'transactions': return 'purple';
        default: return 'blue';
    }
}

function showAddBookModal() {
    document.getElementById('addBookModal').classList.remove('hidden');
}

function showAddUserModal() {
    document.getElementById('addUserModal').classList.remove('hidden');
}

function showBorrowModal() {
    document.getElementById('borrowModal').classList.remove('hidden');
}

function showReturnModal() {
    document.getElementById('returnModal').classList.remove('hidden');
}

function closeModal(modalId) {
    document.getElementById(modalId).classList.add('hidden');
}

async function loadData() {
    try {
        if (currentTab === 'books') {
            const response = await axios.get(`${API_BASE}/books`);
            // Handle both old and new response formats
            const books = response.data.data || response.data.books || response.data || [];
            displayBooks(books);
        } else if (currentTab === 'users') {
            const response = await axios.get(`${API_BASE}/users`);
            // Handle both old and new response formats
            const users = response.data.data || response.data.users || response.data || [];
            displayUsers(users);
        } else if (currentTab === 'transactions') {
            const response = await axios.get(`${API_BASE}/transactions`);
            // Handle both old and new response formats
            const transactions = response.data.data || response.data.transactions || response.data || [];
            displayTransactions(transactions);
        }
        updateStats();
    } catch (error) {
        console.error('Error loading data:', error);
        alert('Error loading data: ' + error.message);
    }
}

async function updateStats() {
    try {
        const booksResponse = await axios.get(`${API_BASE}/books`);
        const usersResponse = await axios.get(`${API_BASE}/users`);
        const transactionsResponse = await axios.get(`${API_BASE}/transactions`);
        
        const books = booksResponse.data.data || booksResponse.data.books || booksResponse.data || [];
        const users = usersResponse.data.data || usersResponse.data.users || usersResponse.data || [];
        const transactions = transactionsResponse.data.data || transactionsResponse.data.transactions || transactionsResponse.data || [];
        
        document.getElementById('totalBooks').textContent = books.length;
        document.getElementById('totalUsers').textContent = users.length;
        document.getElementById('totalTransactions').textContent = transactions.length;
    } catch (error) {
        console.error('Error updating stats:', error);
    }
}

function displayBooks(books) {
    const booksList = document.getElementById('booksList');
    booksList.innerHTML = '';
    
    // Handle different response formats
    if (!books || (Array.isArray(books) && books.length === 0)) {
        booksList.innerHTML = '<p class="text-gray-500 col-span-full text-center">No books found</p>';
        return;
    }
    
    // If books is not an array, it might be wrapped in a data property
    const booksArray = Array.isArray(books) ? books : (books.data || []);
    
    booksArray.forEach(book => {
        const bookCard = document.createElement('div');
        bookCard.className = 'bg-gray-50 rounded-lg p-4 border border-gray-200 hover:shadow-md transition duration-200';
        const adminControls = `
            <h3 class="font-semibold text-gray-800 mb-2">${book.title}</h3>
            <p class="text-gray-600 text-sm mb-1">ISBN: ${book.isbn}</p>
            <p class="text-gray-600 text-sm mb-1">Author: ${book.author}</p>
            <p class="text-gray-600 text-sm mb-1">Year: ${book.publicationYear}</p>
            <p class="text-gray-600 text-sm mb-3">Available: ${book.availableCopies}/${book.totalCopies}</p>
            <div class="flex space-x-2">
                <button onclick="editBook('${book.isbn}')" class="px-3 py-1 bg-blue-600 text-white text-sm rounded hover:bg-blue-700">Edit</button>
                <button onclick="deleteBook('${book.isbn}')" class="px-3 py-1 bg-red-600 text-white text-sm rounded hover:bg-red-700">Delete</button>
            </div>
        `;
        bookCard.innerHTML = adminControls;
        booksList.appendChild(bookCard);
    });
}

function displayUsers(users) {
    const usersList = document.getElementById('usersList');
    usersList.innerHTML = '';
    
    if (users.length === 0) {
        usersList.innerHTML = '<p class="text-gray-500 col-span-full text-center">No users found</p>';
        return;
    }
    
    users.forEach(user => {
        const userCard = document.createElement('div');
        userCard.className = 'bg-gray-50 rounded-lg p-4 border border-gray-200 hover:shadow-md transition duration-200';
        userCard.innerHTML = `
            <h3 class="font-semibold text-gray-800 mb-2">${user.name}</h3>
            <p class="text-gray-600 text-sm mb-1">ID: ${user.userId}</p>
            <p class="text-gray-600 text-sm mb-1">Email: ${user.email}</p>
            <p class="text-gray-600 text-sm mb-1">Phone: ${user.phone}</p>
            <p class="text-gray-600 text-sm mb-3">Books Borrowed: ${user.booksBorrowedCount}</p>
            <div class="flex space-x-2">
                <button onclick="editUser('${user.userId}')" class="px-3 py-1 bg-green-600 text-white text-sm rounded hover:bg-green-700">Edit</button>
                <button onclick="deleteUser('${user.userId}')" class="px-3 py-1 bg-red-600 text-white text-sm rounded hover:bg-red-700">Delete</button>
            </div>
        `;
        usersList.appendChild(userCard);
    });
}

function displayTransactions(transactions) {
    const transactionsList = document.getElementById('transactionsList');
    transactionsList.innerHTML = '';
    
    if (transactions.length === 0) {
        transactionsList.innerHTML = '<p class="text-gray-500 text-center">No transactions found</p>';
        return;
    }
    
    transactions.forEach(transaction => {
        const transactionCard = document.createElement('div');
        transactionCard.className = 'bg-gray-50 rounded-lg p-4 border border-gray-200';
        transactionCard.innerHTML = `
            <div class="flex justify-between items-start mb-2">
                <span class="px-2 py-1 text-xs font-semibold rounded ${transaction.transactionType === 'BORROW' ? 'bg-purple-100 text-purple-800' : 'bg-orange-100 text-orange-800'}">
                    ${transaction.transactionType}
                </span>
                <span class="text-sm text-gray-500">${new Date(transaction.timestamp).toLocaleString()}</span>
            </div>
            <p class="text-gray-700 mb-1">User: ${transaction.userId}</p>
            <p class="text-gray-700 mb-1">Book: ${transaction.bookISBN}</p>
            <p class="text-gray-500 text-sm">ID: ${transaction.transactionId}</p>
        `;
        transactionsList.appendChild(transactionCard);
    });
}

async function addBook(event) {
    event.preventDefault();
    
    const bookData = {
        isbn: document.getElementById('newBookISBN').value,
        title: document.getElementById('newBookTitle').value,
        author: document.getElementById('newBookAuthor').value,
        publicationYear: parseInt(document.getElementById('newBookYear').value),
        copies: parseInt(document.getElementById('newBookCopies').value)
    };
    
    try {
        await axios.post(`${API_BASE}/books`, bookData);
        closeModal('addBookModal');
        loadData();
        // Clear form
        event.target.reset();
    } catch (error) {
        alert('Error adding book: ' + error.response?.data?.error || error.message);
    }
}

async function addUser(event) {
    event.preventDefault();
    
    const userData = {
        userId: document.getElementById('newUserId').value,
        name: document.getElementById('newUserName').value,
        email: document.getElementById('newUserEmail').value,
        phone: document.getElementById('newUserPhone').value
    };
    
    try {
        await axios.post(`${API_BASE}/users`, userData);
        closeModal('addUserModal');
        loadData();
        // Clear form
        event.target.reset();
    } catch (error) {
        alert('Error adding user: ' + error.response?.data?.error || error.message);
    }
}

async function borrowBook(event) {
    event.preventDefault();
    
    const borrowData = {
        userId: document.getElementById('borrowUserId').value,
        bookISBN: document.getElementById('borrowBookISBN').value
    };
    
    try {
        await axios.post(`${API_BASE}/transactions/borrow`, borrowData);
        closeModal('borrowModal');
        loadData();
        // Clear form
        event.target.reset();
        alert('Book borrowed successfully!');
    } catch (error) {
        alert('Error borrowing book: ' + error.response?.data?.error || error.message);
    }
}

async function returnBook(event) {
    event.preventDefault();
    
    const returnData = {
        userId: document.getElementById('returnUserId').value,
        bookISBN: document.getElementById('returnBookISBN').value
    };
    
    try {
        await axios.post(`${API_BASE}/transactions/return`, returnData);
        closeModal('returnModal');
        loadData();
        // Clear form
        event.target.reset();
        alert('Book returned successfully!');
    } catch (error) {
        alert('Error returning book: ' + error.response?.data?.error || error.message);
    }
}

async function performSearch() {
    const query = document.getElementById('searchQuery').value.trim();
    if (!query) return;
    
    try {
        const booksResponse = await axios.get(`${API_BASE}/books/search/${encodeURIComponent(query)}`);
        const usersResponse = await axios.get(`${API_BASE}/users/search/${encodeURIComponent(query)}`);
        
        if (currentTab === 'books') {
            const books = booksResponse.data.data || booksResponse.data.books || booksResponse.data || [];
            displayBooks(books);
        } else if (currentTab === 'users') {
            const users = usersResponse.data.data || usersResponse.data.users || usersResponse.data || [];
            displayUsers(users);
        }
    } catch (error) {
        alert('Error searching: ' + error.message);
    }
}

async function sortBooks() {
    const criteria = document.getElementById('bookSortCriteria').value;
    try {
        const response = await axios.get(`${API_BASE}/sort/books/${criteria}`);
        const books = response.data.data || response.data.books || response.data || [];
        displayBooks(books);
    } catch (error) {
        alert('Error sorting books: ' + error.message);
    }
}

async function sortUsers() {
    const criteria = document.getElementById('userSortCriteria').value;
    try {
        const response = await axios.get(`${API_BASE}/sort/users/${criteria}`);
        const users = response.data.data || response.data.users || response.data || [];
        displayUsers(users);
    } catch (error) {
        alert('Error sorting users: ' + error.message);
    }
}

async function sortTransactions() {
    const criteria = document.getElementById('transactionSortCriteria').value;
    try {
        const response = await axios.get(`${API_BASE}/sort/transactions/${criteria}`);
        const txns = response.data.data || response.data.transactions || response.data || [];
        displayTransactions(txns);
    } catch (error) {
        alert('Error sorting transactions: ' + error.message);
    }
}

async function editBook(isbn) {
    // For simplicity, we'll just show the book details in an alert
    // In a real application, you'd show an edit modal
    try {
        const response = await axios.get(`${API_BASE}/books/${isbn}`);
        const book = response.data.data || response.data.book || response.data;
        alert(`Edit functionality for book: ${book.title}\nISBN: ${book.isbn}\nAuthor: ${book.author}\nYear: ${book.publicationYear}\nCopies: ${book.totalCopies}`);
    } catch (error) {
        alert('Error loading book details: ' + (error.response?.data?.error || error.message));
    }
}

async function editUser(userId) {
    // For simplicity, we'll just show the user details in an alert
    // In a real application, you'd show an edit modal
    try {
        const response = await axios.get(`${API_BASE}/users/${userId}`);
        const user = response.data.data || response.data.user || response.data;
        alert(`Edit functionality for user: ${user.name}\nID: ${user.userId}\nEmail: ${user.email}\nPhone: ${user.phone}\nBooks Borrowed: ${user.booksBorrowedCount}`);
    } catch (error) {
        alert('Error loading user details: ' + (error.response?.data?.error || error.message));
    }
}

async function deleteBook(isbn) {
    if (confirm('Are you sure you want to delete this book?')) {
        try {
            await axios.delete(`${API_BASE}/books/${isbn}`);
            loadData();
            alert('Book deleted successfully!');
        } catch (error) {
            alert('Error deleting book: ' + (error.response?.data?.error || error.message));
        }
    }
}

async function deleteUser(userId) {
    if (confirm('Are you sure you want to delete this user?')) {
        try {
            await axios.delete(`${API_BASE}/users/${userId}`);
            loadData();
            alert('User deleted successfully!');
        } catch (error) {
            alert('Error deleting user: ' + (error.response?.data?.error || error.message));
        }
    }
}

// Initialize the application
document.addEventListener('DOMContentLoaded', function() {
    loadData();
    
    // Add enter key support for search
    document.getElementById('searchQuery').addEventListener('keypress', function(e) {
        if (e.key === 'Enter') {
            performSearch();
        }
    });
});

// No auth flows – reverted to original quick actions