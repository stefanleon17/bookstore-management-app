#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<sstream>

using namespace std;

class Book
{
protected:
	string title, author, ISBN;
	int numPages, quantity, ID;
	double price;
public:
	Book(string title = "", string author = "", string ISBN = "", int numPages = 0, double price = 0, int quantity = 0)
		: title(title), author(author), ISBN(ISBN), numPages(numPages), price(price), quantity(quantity) {}
	virtual void display()			///displays the details of the book on the screen
	{
		cout << title << ", by " << author << endl << quantity << " in stock \n$" << price << endl;
		cout << "ID: " << ID;
	}

	//we introduce an ID data member with getters and setters to make accessing the elements easier
	void setID(int id)
	{
		ID = id;
	}

	int getID()
	{
		return ID;
	}

	void sell()    //lowers the quantity and displays the price that needs to be paid
	{
		if (quantity > 0)
		{
			quantity--;
			cout << "$ " << price << endl;
		}
		else
			cout << "There are no copies in stock!";
	}

	bool searchISBN(string searchString)						//KMP pattern search algorithm 
	{
		if (ISBN.size() < searchString.size())
			return 0;

		int* prefix = new int[searchString.size()];
		prefix[0] = 0;

		for (int i = 1; i < searchString.size(); i++)
		{
			int var = prefix[i - 1];
			while (searchString[i] != searchString[var] && var != 0)
				var = prefix[var - 1];
			if (searchString[i] == searchString[var])
				prefix[i] = var + 1;
			else prefix[i] = 0;
		}

		int textPos = 0, searchPos = 0;
		while (textPos < ISBN.size())
		{
			if (searchString[searchPos] == ISBN[textPos])
			{
				textPos++;
				searchPos++;
			}

			if (searchPos == searchString.size())
				return 1;
			else if (searchString[searchPos] != ISBN[textPos])
				if (searchPos == 0)
					textPos++;
				else
					searchPos = prefix[searchPos - 1];
		}

		return 0;
	}
	bool searchTitle(string searchString)						//BM pattern search algorithm
	{
		if (title.size() < searchString.size())
			return 0;

		int start = 0;
		short int positions[256];

		for (int i = 0; i < 256; i++)
			positions[i] = 0;

		for (int i = 0; i < searchString.size(); i++)
			positions[searchString[i]] = i;

		while (start <= title.size() - searchString.size())
		{
			int i = searchString.size() - 1;
			while ((i >= 0) && (searchString[i] == title[i + start]))
				i--;

			if (i < 0)
			{
				return 1;
			}
			else start += max(1, i - positions[title[i + start]]);
		}
	
		return 0;
	}			
	bool searchAuthor(string searchString)					//BM pattern search algorithm
	{ 
		if (author.size() < searchString.size())
			return 0;

		int start = 0;
		short int positions[256];

		for (int i = 0; i < 256; i++)
			positions[i] = 0;

		for (int i = 0; i < searchString.size(); i++)
			positions[searchString[i]] = i;

		while (start <= author.size() - searchString.size())
		{
			int i = searchString.size() - 1;
			while ((i >= 0) && (searchString[i] == author[i + start]))
				i--;

			if (i < 0)
			{
				return 1;
			}
			else start += max(1, i - positions[author[i + start]]);
		}

		return 0;

	}		
	virtual bool search(string searchString)		 //the search method returns true if any of the other methods return true
	{
		return searchISBN(searchString) || searchTitle(searchString) || searchAuthor(searchString);
	}

	virtual void recommend() {}		//polymorphic function; implements quicksort using a vector
	
	//the following virtual and non-virtual getters are necessary in order to build the subsequent recommend methods
	double getPrice()
	{
		return price;
	}
	string getTitle()
	{
		return title;
	}
	string getAuthor()
	{
		return author;
	}
	string getISBN()
	{
		return ISBN;
	}
	int getNumPages()
	{
		return numPages;
	}
	int getQuantity()
	{
		return quantity;
	}
	virtual string getTheme()
	{
		return "";
	}
	virtual string getType() { return ""; }		
	virtual string getCategory() { return ""; }
	virtual string getPublisher()
	{
		return "";
	}
	virtual bool isBestseller()
	{
		return false;
	}
	virtual bool isColoured()
	{
		return false;
	}
	struct Date { int day = 0; int month = 0; int year = 0; } date;
};

vector <Book*> bookDB;		//vector that will store all the objects representing the books

int partition (vector<int> &scores, int low, int high)
{

	int pivot = scores[low];

	int count = 0;
	for (int i = low + 1; i <= high; i++) 
	{
		if (scores[i] <= pivot)
			count++;
	}

	int position = low + count;
	swap(scores[position], scores[low]);
	swap(bookDB[position], bookDB[low]);

	int i = low, j = high;

	while (i < position && j > position) 
	{
		while (scores[i] <= pivot) 
			i++;

		while (scores[j] > pivot)
			j--;

		if (i < position && j > position) 
		{
			swap(scores[i++], scores[j--]);
			swap(bookDB[i], bookDB[j]);
		}
	}
	
	return position;
}

void quicksort(vector<int> &scores, int low, int high)
{
	if (low < high)
	{
		int position = partition(scores, low, high);

		quicksort(scores, low, position - 1);

		quicksort(scores, position + 1, high);
	}
}

class LiteraryBook : public Book
{
protected:
	string genre;
	string getCategory()
	{
		return "literary";
	}
	string getTheme()
	{
		return genre;
	}
public:
	LiteraryBook(string title = "", string author = "", string ISBN = "", int numPages = 0, double price = 0, string genre = "", int quantity = 0)
		: Book(title, author, ISBN, numPages, price, quantity), genre(genre) {}

	bool searchGenre(string searchString)			 //BM pattern search algorithm
	{ 
		if (genre.size() < searchString.size())
			return 0;

		int start = 0;
		short int positions[256];

		for (int i = 0; i < 256; i++)
			positions[i] = 0;

		for (int i = 0; i < searchString.size(); i++)
			positions[searchString[i]] = i;

		while (start <= genre.size() - searchString.size())
		{
			int i = searchString.size() - 1;
			while ((i >= 0) && (searchString[i] == genre[i + start]))
				i--;

			if (i < 0)
			{
				return 1;
			}
			else start += max(1, i - positions[genre[i + start]]);
		}

		return 0;

	}		
	virtual bool search(string searchString)
	{
		return Book::search(searchString) || searchGenre(searchString);
	}
};

class NonLiteraryBook : public Book
{
protected:
	string topic;
	string getCategory()
	{
		return "non-literary";
	}
	string getTheme()
	{
		return topic;
	}
public:
	
	
	NonLiteraryBook(string title = "", string author = "", string ISBN = "", int numPages = 0, double price = 0, string topic = "", int day = 1, int month = 1, int year = 1, int quantity = 0)
		: Book(title, author, ISBN, numPages, price, quantity), topic(topic)
	{
		date.day = day;
		date.month = month;
		date.year = year;
	}

	void display()
	{
		Book::display();
		cout << "\nDate Published: " << date.day << "." << date.month << "." << date.year << endl;
	}

	bool searchTopic(string searchString) 		//BM pattern search algorithm
	{
		if (topic.size() < searchString.size())
			return 0;

		int start = 0;
		short int positions[256];

		for (int i = 0; i < 256; i++)
			positions[i] = 0;

		for (int i = 0; i < searchString.size(); i++)
			positions[searchString[i]] = i;

		while (start <= topic.size() - searchString.size())
		{
			int i = searchString.size() - 1;
			while ((i >= 0) && (searchString[i] == topic[i + start]))
				i--;

			if (i < 0)
			{
				return 1;
			}
			else start += max(1, i - positions[topic[i + start]]);
		}

		return 0;

	}

	virtual bool search(string searchString)
	{
		return Book::search(searchString) || searchTopic(searchString);
	}
};

class FictionBook : public LiteraryBook
{
private:
	bool bestseller;
	string getType()
	{
		return "fiction";
	}
public:
	FictionBook(string title = "", string author = "", string ISBN = "", int numPages = 0, double price = 0, string genre = "", bool bestseller = 0, int quantity = 0)
		:LiteraryBook(title, author, ISBN, numPages, price, genre, quantity), bestseller(bestseller) {}

	void recommend() 
	{
		vector<int> scores(bookDB.size(), 0);

		for (int i = 0; i < bookDB.size(); i++)
		{
			if (bookDB[i]->getCategory() == this->getCategory())
				scores[i] += 5;
			if (bookDB[i]->getType() == this->getType())
				scores[i] += 5;
			if (bookDB[i]->getAuthor() == this->author)
				scores[i] += 10;
			if (bookDB[i]->getTheme() == this->genre)
				scores[i] += 15;
			if (abs(bookDB[i]->getNumPages() - this->numPages) < 90)
				scores[i] += 5;
			if (bookDB[i]->isBestseller())
				scores[i] += 7;
		}

		quicksort(scores, 0, bookDB.size() - 1);
	}
};

class NonFictionBook : public LiteraryBook
{
private:
	bool bestseller;
	string getType()
	{
		return "non-fiction";
	}
public:
	NonFictionBook(string title = "", string author = "", string ISBN = "", int numPages = 0, double price = 0, string genre = "", bool bestseller = 0, int quantity = 0)
		:LiteraryBook(title, author, ISBN, numPages, price, genre, quantity), bestseller(bestseller) {}

	void recommend()
	{
		vector<int> scores(bookDB.size(), 0);

		for (int i = 0; i < bookDB.size(); i++)
		{
			if (bookDB[i]->getCategory() == this->getCategory())
				scores[i] += 3;
			if (bookDB[i]->getType() == this->getType())
				scores[i] += 10;
			if (bookDB[i]->getAuthor() == this->author)
				scores[i] += 15;
			if (bookDB[i]->getTheme() == this->genre)
				scores[i] += 15;
			if (bookDB[i]->isBestseller())
				scores[i] += 5;
		}

		quicksort(scores, 0, bookDB.size() - 1);
	}
};

class ComicBook : public LiteraryBook
{
private:
	string getType()
	{
		return "comicbook";
	}
	bool coloured;
public:
	ComicBook(string title = "", string author = "", string ISBN = "", int numPages = 0, double price = 0, string genre = "", bool coloured = false, int quantity = 0)
		:LiteraryBook(title, author, ISBN, numPages, price, genre, quantity), coloured(coloured) {}

	void recommend()
	{
		vector<int> scores(bookDB.size(), 0);

		for (int i = 0; i < bookDB.size(); i++)
		{
			if (bookDB[i]->getCategory() == this->getCategory())
				scores[i] += 1;
			if (bookDB[i]->getType() == this->getType())
				scores[i] += 25;
			if (bookDB[i]->getAuthor() == this->author)
				scores[i] += 10;
			if (bookDB[i]->getTheme() == this->genre)
				scores[i] += 10;
			if (bookDB[i]->getNumPages() - this->numPages < 50)
				scores[i] += 5;
		}

		quicksort(scores, 0, bookDB.size() - 1);
	}
};

class PoetryBook : public LiteraryBook
{
private:
	string getType()
	{
		return "poetry";
	}
public:
	PoetryBook(string title = "", string author = "", string ISBN = "", int numPages = 0, double price = 0, string genre = "", int quantity = 0)
		: LiteraryBook(title, author, ISBN, numPages, price, genre, quantity) {}

	void recommend()
	{
		vector<int> scores(bookDB.size(), 0);

		for (int i = 0; i < bookDB.size(); i++)
		{
			if (bookDB[i]->getCategory() == this->getCategory())
				scores[i] += 15;
			if (bookDB[i]->getType() == this->getType())
				scores[i] += 20;
			if (bookDB[i]->getAuthor() == this->author)
				scores[i] += 30;
			if (bookDB[i]->getTheme() == this->genre)
				scores[i] += 8;
			if (abs(bookDB[i]->getNumPages() - this->numPages) < 60)
				scores[i] += 10;
			if (bookDB[i]->getType() == "comicbook")
				scores[i] -= 10;
		}

		quicksort(scores, 0, bookDB.size() - 1);
	}
};

class SkillBook : public NonLiteraryBook
{
private:
	string getType()
	{
		return "skill";
	}
public:
	SkillBook(string title = "", string author = "", string ISBN = "", int numPages = 0, double price = 0, string topic = "", int day = 1, int month = 1, int year = 1, int quantity = 0)
		:NonLiteraryBook(title, author, ISBN, numPages, price, topic, day, month, year, quantity) {}

	void recommend()
	{
		vector<int> scores(bookDB.size(), 0);

		for (int i = 0; i < bookDB.size(); i++)
		{
			if (bookDB[i]->getCategory() == this->getCategory())
				scores[i] += 5;
			if (bookDB[i]->getType() == this->getType())
				scores[i] += 10;
			if (bookDB[i]->getAuthor() == this->author)
				scores[i] += 3;
			if (bookDB[i]->getTheme() == this->topic)
				scores[i] += 35;
			if (bookDB[i]->date.year > 2010)
				scores[i] += 15;
			if (bookDB[i]->date.year > 2020)
				scores[i] += 15;
		}

		quicksort(scores, 0, bookDB.size() - 1);
	}
};

class ScientificBook : public NonLiteraryBook
{
private:
	string doi;
	string getType()
	{
		return "scientific";
	}
public:
	ScientificBook(string title = "", string author = "", string ISBN = "", int numPages = 0, double price = 0, string topic = "", int day = 1, int month = 1, int year = 1, string doi = "", int quantity = 0)
		: NonLiteraryBook(title, author, ISBN, numPages, price, topic, day, month, year, quantity), doi(doi) {}

	bool searchDOI(string searchString)			//KMP pattern search algorithm
	{
		if (doi.size() < searchString.size())
			return 0;

		int* prefix = new int[searchString.size()];
		prefix[0] = 0;

		for (int i = 1; i < searchString.size(); i++)
		{
			int var = prefix[i - 1];
			while (searchString[i] != searchString[var] && var != 0)
				var = prefix[var - 1];
			if (searchString[i] == searchString[var])
				prefix[i] = var + 1;
			else prefix[i] = 0;
		}

		int textPos = 0, searchPos = 0;
		while (textPos < doi.size())
		{
			if (searchString[searchPos] == doi[textPos])
			{
				textPos++;
				searchPos++;
			}

			if (searchPos == searchString.size())
				return 1;
			else if (searchString[searchPos] != doi[textPos])
				if (searchPos == 0)
					textPos++;
				else
					searchPos = prefix[searchPos - 1];
		}

		return 0;
	}
	bool search(string searchString)
	{
		return  NonLiteraryBook::search(searchString) || searchDOI(searchString);
	}

	void recommend()
	{
		vector<int> scores(bookDB.size(), 0);

		for (int i = 0; i < bookDB.size(); i++)
		{
			if (bookDB[i]->getCategory() == this->getCategory())
				scores[i] += 15;
			if (bookDB[i]->getType() == this->getType())
				scores[i] += 25;
			if (bookDB[i]->getAuthor() == this->author)
				scores[i] += 7;
			if (bookDB[i]->getTheme() == this->topic)
				scores[i] += 35;
			if (bookDB[i]->date.year > 2000)
				scores[i] += 15;
			if (bookDB[i]->date.year > 2015)
				scores[i] += 20;
		}

		quicksort(scores, 0, bookDB.size() - 1);
	}
};

class Magazine : public NonLiteraryBook
{
private:
	string publisher;
	string getType()
	{
		return "magazine";
	}
public:
	enum Frequency { Daily, Weekly, Biweekly, Monthly, Annually } frequency;

	Magazine(string title = "", string author = "", string ISBN = "", int numPages = 0, double price = 0, string topic = "", int day = 1, int month = 1, int year = 1, string publisher = "", Frequency frequency = Daily, int quantity = 0)
		: NonLiteraryBook(title, author, ISBN, numPages, price, topic, day, month, year, quantity), frequency(frequency), publisher(publisher) {}

	bool searchPublisher(string searchString)			//BM pattern search algorithm
	{
		if (publisher.size() < searchString.size())
			return 0;

		int start = 0;
		short int positions[256];

		for (int i = 0; i < 256; i++)
			positions[i] = 0;

		for (int i = 0; i < searchString.size(); i++)
			positions[searchString[i]] = i;

		while (start <= publisher.size() - searchString.size())
		{
			int i = searchString.size() - 1;
			while ((i >= 0) && (searchString[i] == publisher[i + start]))
				i--;

			if (i < 0)
			{
				return 1;
			}
			else start += max(1, i - positions[publisher[i + start]]);
		}

		return 0;

	}
	bool search(string searchString)
	{
		return NonLiteraryBook::search(searchString) || searchPublisher(searchString);
	}

	void recommend()
	{
		vector<int> scores(bookDB.size(), 0);

		for (int i = 0; i < bookDB.size(); i++)
		{
			if (bookDB[i]->getCategory() == this->getCategory())
				scores[i] += 10;
			if (bookDB[i]->getType() == this->getType())
				scores[i] += 15;
			if (bookDB[i]->getAuthor() == this->author)
				scores[i] += 5;
			if (bookDB[i]->getTheme() == this->topic)
				scores[i] += 12;
			if (bookDB[i]->date.year < 2021)
				scores[i] -= 10;
			if (bookDB[i]->getPublisher() == this->publisher)
				scores[i] += 8;
		}

		quicksort(scores, 0, bookDB.size() - 1);
	}
};

void read()		//this function handles data input from the file containing all the books
{
	string fileName = "booksDatabase.txt";
	ifstream in;

	in.open(fileName);
	if (!in.is_open())
		cout << "Error: could not open file " << fileName;

	int count = 0;
	while (!in.eof())
	{
		string input;
		getline(in, input);
		istringstream issInput(input);

		string token;
		getline(issInput, token, ',');

		if (token == "fiction")
		{
			int i = 1;
			string title, author, ISBN, numPagesString, priceString, genre, quantityString;
			bool bestseller = 0;
			while (getline(issInput, token, ','))
			{
				switch (i)
				{
				case(1): {title = token; i++; break; }
				case(2): {author = token; i++; break; }
				case(3): {ISBN = token; i++; break; }
				case(4): {numPagesString = token; i++; break; }
				case(5): {priceString = token; i++; break; }
				case(6): {genre = token; i++; break; }
				case(7): {if (token == "bestseller") bestseller = 1; i++; break; }
				case(8): {quantityString = token; i++; break; }
				}
			}

			int numPages = stoi(numPagesString);
			double price = stod(priceString);
			int quantity = stoi(quantityString);

			bookDB.push_back(new FictionBook(title, author, ISBN, numPages, price, genre, bestseller, quantity));
			bookDB[count]->setID(count);
			count++;
		}
		else if (token == "comicbook")
		{
			int i = 1;
			string title, author, ISBN, numPagesString, priceString, genre, quantityString;
			bool coloured = false;
			while (getline(issInput, token, ','))
			{
				switch (i)
				{
				case(1): {title = token; i++; break; }
				case(2): {author = token; i++; break; }
				case(3): {ISBN = token; i++; break; }
				case(4): {numPagesString = token; i++; break; }
				case(5): {priceString = token; i++; break; }
				case(6): {genre = token; i++; break; }
				case(7): { if (token == "coloured") coloured = true; i++; break; }
				case(8): {quantityString = token; i++; break; }
				}
			}

			int numPages = stoi(numPagesString);
			double price = stod(priceString);
			int quantity = stoi(quantityString);

			bookDB.push_back(new ComicBook(title, author, ISBN, numPages, price, genre, coloured, quantity));
			bookDB[count]->setID(count);
			count++;
		}
		else if (token == "non-fiction")
		{
			int i = 1;
			string title, author, ISBN, numPagesString, priceString, genre, quantityString;
			bool bestseller = 0;
			while (getline(issInput, token, ','))
			{
				switch (i)
				{
				case(1): {title = token; i++; break; }
				case(2): {author = token; i++; break; }
				case(3): {ISBN = token; i++; break; }
				case(4): {numPagesString = token; i++; break; }
				case(5): {priceString = token; i++; break; }
				case(6): {genre = token; i++; break; }
				case(7): {if (token == "bestseller") bestseller = 1; i++; break; }
				case(8): {quantityString = token; i++; break; }
				}
			}

			int numPages = stoi(numPagesString);
			double price = stod(priceString);
			int quantity = stoi(quantityString);

			bookDB.push_back(new NonFictionBook(title, author, ISBN, numPages, price, genre, bestseller, quantity));
			bookDB[count]->setID(count);
			count++;
		}
		else if (token == "poetry")
		{
			int i = 1;
			string title, author, ISBN, numPagesString, priceString, genre, quantityString;
			while (getline(issInput, token, ','))
			{
				switch (i)
				{
				case(1): {title = token; i++; break; }
				case(2): {author = token; i++; break; }
				case(3): {ISBN = token; i++; break; }
				case(4): {numPagesString = token; i++; break; }
				case(5): {priceString = token; i++; break; }
				case(6): {genre = token; i++; break; }
				case(7): {quantityString = token; i++; break; }
				}
			}

			int numPages = stoi(numPagesString);
			double price = stod(priceString);
			int quantity = stoi(quantityString);

			bookDB.push_back(new PoetryBook(title, author, ISBN, numPages, price, genre, quantity));
			bookDB[count]->setID(count);
			count++;
		}
		else if (token == "skillbook")
		{
			int i = 1;
			string title, author, ISBN, numPagesString, priceString, topic, quantityString, dayString, monthString, yearString;
			while (getline(issInput, token, ','))
			{
				switch (i)
				{
				case(1): {title = token; i++; break; }
				case(2): {author = token; i++; break; }
				case(3): {ISBN = token; i++; break; }
				case(4): {numPagesString = token; i++; break; }
				case(5): {priceString = token; i++; break; }
				case(6): {topic = token; i++; break; }
				case(7): {dayString = token; i++; break; }
				case(8): {monthString = token; i++; break; }
				case(9): {yearString = token; i++; break; }
				case(10): {quantityString = token; i++; break; }
				}
			}

			int numPages = stoi(numPagesString);
			double price = stod(priceString);
			int quantity = stoi(quantityString);
			int day = stoi(dayString);
			int month = stoi(monthString);
			int year = stoi(yearString);

			bookDB.push_back(new SkillBook(title, author, ISBN, numPages, price, topic, day, month, year, quantity));
			bookDB[count]->setID(count);
			count++;
		}
		else if (token == "scientific")
		{
			int i = 1;
			string title, author, ISBN, numPagesString, priceString, topic, quantityString, dayString, monthString, yearString, doi;
			while (getline(issInput, token, ','))
			{
				switch (i)
				{
				case(1): {title = token; i++; break; }
				case(2): {author = token; i++; break; }
				case(3): {ISBN = token; i++; break; }
				case(4): {numPagesString = token; i++; break; }
				case(5): {priceString = token; i++; break; }
				case(6): {topic = token; i++; break; }
				case(7): {dayString = token; i++; break; }
				case(8): {monthString = token; i++; break; }
				case(9): {yearString = token; i++; break; }
				case(10): {doi = token; i++; break; }
				case(11): {quantityString = token; i++; break; }
				}
			}

			int numPages = stoi(numPagesString);
			double price = stod(priceString);
			int quantity = stoi(quantityString);
			int day = stoi(dayString);
			int month = stoi(monthString);
			int year = stoi(yearString);

			bookDB.push_back(new ScientificBook(title, author, ISBN, numPages, price, topic, day, month, year, doi, quantity));
			bookDB[count]->setID(count);
			count++;
		}
		else if (token == "magazine")
		{
			int i = 1;
			string title, author, ISBN, numPagesString, priceString, topic, quantityString, dayString, monthString, yearString, publisher;
			Magazine::Frequency freq = Magazine::Daily;
			while (getline(issInput, token, ','))
			{
				switch (i)
				{
				case(1): {title = token; i++; break; }
				case(2): {author = token; i++; break; }
				case(3): {ISBN = token; i++; break; }
				case(4): {numPagesString = token; i++; break; }
				case(5): {priceString = token; i++; break; }
				case(6): {topic = token; i++; break; }
				case(7): {dayString = token; i++; break; }
				case(8): {monthString = token; i++; break; }
				case(9): {yearString = token; i++; break; }
				case(10): {publisher = token; i++; break; }
				case(11):
				{
					if (token == "Weekly")
						freq = Magazine::Weekly;
					else if (token == "Monthly")
						freq = Magazine::Monthly;
					else if (token == "Biweekly")
						freq = Magazine::Biweekly;
					else if (token == "Annually")
						freq = Magazine::Annually;
					i++;
					break;
				}
				case(12): {quantityString = token; i++; break; }
				}
			}

			int numPages = stoi(numPagesString);
			double price = stod(priceString);
			int quantity = stoi(quantityString);
			int day = stoi(dayString);
			int month = stoi(monthString);
			int year = stoi(yearString);

			bookDB.push_back(new Magazine(title, author, ISBN, numPages, price, topic, day, month, year,publisher,freq, quantity));
			bookDB[count]->setID(count);
			count++;
		}
	}
}

void displayMenu()
{
	cout << "==================== Welcome to the bookstore management system ====================" << endl;
	cout << " ==== Please input the number in front of the option you would like to select ===== " << endl;
	cout << "  1. Sell a book" << endl;
	cout << "  2. Search for a book" << endl;
	cout << "  3. Make a book recommendation to a customer" << endl;
	
	int input;
	cin >> input;
	switch (input)
	{
		case(1): 
		{
			cout << "Please input the ID of the book you would like to sell (Enter -1 to go back): ";
			cin >> input;
			if (input == -1)
			{
				cout << endl;
				displayMenu;
			}
			else 
			{
				bookDB[input]->sell();
				displayMenu();
			}
			break;
		}
		case(2):
		{
			cout << "Enter the search term (Enter -1 to go back): ";
			string search;
			cin >> search;
			if (search == "-1")
			{
				cout << endl;
				displayMenu();
			}
			else
			{
				for (int i = 0; i < bookDB.size(); i++)
					if (bookDB[i]->search(search))
					{
						cout << endl;
						bookDB[i]->display();
						cout << endl;
					}
				displayMenu();
			}
			break;
		}
		case (3):
		{
			cout << "Enter the ID of your favourite book (Enter -1 to go back and search for it, if needed): ";
			cin >> input;
			if (input == -1)
			{
				cout << endl;
				displayMenu();
			}
			else
			{
				bookDB[input]->recommend();
				cout << endl;
				bookDB[0]->display();

				cout << "\n\nGet the next best match? (Enter 1 for yes, or -1 for no)";
				cin >> input;
				int i = 1;
				while (input != -1)
				{
					cout << endl;
					bookDB[i]->display();
					i++;
					cout << "\n\nGet the next best match? (Enter 1 for yes, or -1 for no)";
					cin >> input;
				}
			}
		}
	}

	cout << "\nThank you for using our bookstore management system!\n";
}

int main()
{
	read();
	displayMenu();
}