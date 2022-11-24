# bookstore-mangement-app
The program is meant to make the job of managing a bookstore easier. It implements a 3-level hierarchy of classes, as well as an algorithm for recommending a book, based on the preferences of a certain customer.

# User Guide
Once the program is run, the user is greeted with the following message:

============= Welcome to the bookstore management system =============

==== Please input the number in front of the option you would like to select ===== 
  1. Sell a book
  2. Search for a book
  3. Make a book recommendation to a customer

To make use of the program, the user has to input a number (1-3), corresponding with the action they would like to perform. The program utilizes program-defined IDs for the books, to make accessing them easier, so it is recommended that a user searches for a book first, to get its ID, before performing any of the other actions.
After performing any of the actions, the program will close. In order to perform another action, the program must be run again.


Actions

1.	Upon inputting 1, the user will be prompted with a message asking for the ID of the book that they would like to sell. Entering an integer will access the book entry with the corresponding ID, lower the quantity in stock and output the price which the customer needs to pay for the purchase. Inputting -1 will instead lead the user back to the previous menu screen.
2.	Upon inputting 2, the user will be asked to enter a search term; this can be part of the title, author or ISBN field, or in the case of other specific items, it can refer to the publisher of a magazine or to the DOI number of a scientific article. After entering the search term and pressing the “Enter” key, the user will receive a list of all the books currently in the database that contain the term in any of the fields mentioned. The searching is case sensitive, so caution is advised when writing down the search string. This will also provide the user with the books’ IDs, so it is recommended that the user performs this action before any of the other ones. Inputting -1 will instead lead the user back to the previous menu screen.
3.	Upon inputting 3, the user will be asked to enter the ID of his – or more likely the customer’s – favorite book. The program will then execute a sorting algorithm and suggest books for purchase, in the order of their likelihood to be enjoyed, based on the ID of the initial book entered. In case that the first book recommended does not appeal to the customer for whatever reason, or in case they have read it before, the user can then input 1 to continue going through the list of books available, in order of their relevance, until an appropriate one has been found. Inputting -1 when prompted for an ID will instead lead the user back to the previous menu screen; entering -1 at any point after that will instead end the execution of the program. 


This following are examples of interactions with the program, to better illustrate the recommended usage practices:
A customer walks into the shop, and decides on a book to buy. The sales clerk then runs the program, searches for the book by pressing “2” and entering part of the title as a search term. They note the ID of the book, then run the program again, instead selecting “1” and entering it when prompted. They then ask the customer to pay the appropriate amount.
A customer walks into the shop, but is not decided on what book to buy. They ask the sales clerk for advice, and in response the sales clerk asks them for some of their favorite books; once they mention one that exists in the shop, the sales clerk runs the program, presses “2” to search for the book’s ID, then runs the program again, now pressing “3” when prompted and entering the book’s ID. They then look through the books in order of their relevance to the customer, stopping whenever the customer is intrigued by one enough to buy it. They then note its ID and input “-1”, thus ending the execution of the program, and then run it again, this time pressing “1” and entering the desired book’s ID to make the sale.


# Data Structures
The following is a list of the data structures used in the bookstore management app.

	class Book
	{
	protected:
		string ID, title, author, ISBN;
		int numPages, quantity;
		double price;
	public:
		Book(string title = "", string author = "", string ISBN = "", int numPages = 0, double price = 0, int quantity = 0);
		void display();		//displays the details of the book on the screen

		virtual void recommend();	 //polymorphic function; implements quicksort algorithm

		void sell();		//lowers the quantity and displays the price that needs to be paid

		bool searchISBN(string search);	//KMP pattern search algorithm
		bool searchName(string search);		//BM pattern search algorithm
		bool searchAuthor(string search);	//BM pattern search algorithm
		virtual bool search(string search);	//the search method returns true if any of the other methods return true
	};


	class LiteraryBook : public Book
	{
	protected:
		string genre;
		const string category = "literary";
	public:
		LiteraryBook(string title = "", string author = "", string ISBN = "", int numPages = 0, double price = 0, int quantity = 0, string genre = "");

		bool searachGenre(string search);	//BM pattern search algorithm
		virtual bool search(string search);
	};

	class NonLiteraryBook : public Book
	{
	protected:
		string topic;
		const string category = "non-literary";
		struct Date { int day; int month; int year; } date;
	public:
		NonLiteraryBook(string title = "", string author = "", string ISBN = "", int numPages = 0, double price = 0, int quantity = 0, string topic = "", int day = 1, int month = 1, int year = 1);

		bool searchTopic(string search);	 //BM pattern search algorithm
		virtual bool search(string search);
	};

	class FictionBook : public LiteraryBook
	{
	private:
		const string type = "fiction";
		bool bestseller;
	public:
		FictionBook(string title = "", string author = "", string ISBN = "", int numPages = 0, double price = 0, int quantity = 0, string genre = "", bool bestseller = 0);

		void recommend();	
	};

	class NonFictionBook : public LiteraryBook
	{
	private:
		const string type = "non-fiction";
		bool bestseller;
	public:
		NonFictionBook(string title = "", string author = "", string ISBN = "", int numPages = 0, double price = 0, int quantity = 0, string genre = "", bool bestseller = 0);

		void recommend();
	};

	class ComicBook : public LiteraryBook
	{
	private:
		const string type = "comicbook";
	public:
		enum Colour { Coloured, BlackAndWhite } colour;
		ComicBook(string title = "", string author = "", string ISBN = "", int numPages = 0, double price = 0, int quantity = 0, string genre = "", Colour colour = BlackAndWhite);

		void recommend();

	};

	class PoetryBook : public LiteraryBook
	{
	private:
		const string type = "poetry";
	public:
		PoetryBook(string title = "", string author = "", string ISBN = "", int numPages = 0, double price = 0, int quantity = 0, string genre = "");

		void recommend();
	};

	class SkillBook : public NonLiteraryBook
	{
	private:
		const string type = "skill";
	public:
		SkillBook(string title = "", string author = "", string ISBN = "", int numPages = 0, double price = 0, int quantity = 0, string topic = "", int day = 1, int month = 1, int year = 1);

		void recommend();
	};

	class ScientificBook : public NonLiteraryBook
	{
	private:
		const string type = "scientific";
		string doi;
	public:
		ScientificBook(string title = "", string author = "", string ISBN = "", int numPages = 0, double price = 0, int quantity = 0, string topic = "", int day = 1, int month = 1, int year = 1, string doi = "");

		bool searchDOI(string search);	//KMP pattern search algorithm
		bool search(string search);

		void recommend();
	};

	class Magazine : public NonLiteraryBook
	{
	private:
		const string type = "magazine";
		string publisher;
	public:
		enum Frequency { Daily, Weekly, Biweekly, Monthly, Annualy } frequency;

		Magazine(string title = "", string author = "", string ISBN = "", int numPages = 0, double price = 0, int quantity = 0, string topic = "", int day = 1, int month = 1, int year = 1, string publisher = "", Frequency frequency = Daily);

		bool searchPublisher(string search);
		bool search(string search);

		void recommend();
	}; 
All of the methods used for some sort of “search” are implemented using either KMP pattern matching algorithms (for strings that use a smaller alphabet, such as an ISBN or DOI code), with a worst-case time complexity of O(n), or a BM pattern matching algorithm (for strings that may use up all of the English alphabet, such as the title of a book or the name of an author), with an average time complexity of O(n). The BM algorithm can potentially be much faster than the KMP algorithm (which is more reliable), but only in cases where letters can be skipped, so it will not be used for strings that use very small and limited alphabets, like the ISBN and DOI codes.
The program uses a vector of Book type pointers to store references to all the books in the database. Adding a new element to the vector is done in the usual time complexity, either O(1) if there is empty pre-allocated space in the vector, or O(n), if the vector needs to be resized to add the element. Deleting an element is similarly done in O(n) time (it will be therefore recommended that elements are not generally deleted, even if their quantity reaches 0).
The “recommend” polymorphic function has a different implementation for each of the seven subclasses. It attributes a compatibility score to each book stored in the vector (and store these values in a separate array), based on different parameters, with different weights, and then implements a quicksort algorithm to sort the elements in the vector by their compatibility score. This operation is done, as per the quicksort algorithm, in O(n*log n) time complexity.
The program uses a vector as, despite its inefficiency in adding and removing elements, it allows for an efficient quicksort algorithm to be performed, which allows the user to view as many books as they would like, ordered by the “recommend” method, as opposed to only seeing a constant, predetermined number of them. This way, they can all be accessed, in order of their relevance to the customer. 

