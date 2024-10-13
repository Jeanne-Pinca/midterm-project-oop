#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

// class used to represent the items individually
class Item {
private:
    string itemId;
    string itemName;
    int itemQuantity;
    double itemPrice;
    string itemCategory;
    
public:
    // Constructor using initialization list
    Item(const string& id, const string& name, int quantity, double price, const string& category) 
        : itemId(id), itemName(name), itemQuantity(quantity), itemPrice(price), itemCategory(category) {}

    const string& getId() const { return itemId; }
    const string& getName() const { return itemName; }
    int getQuantity() const { return itemQuantity; }
    double getPrice() const { return itemPrice; }
    const string& getCategory() const { return itemCategory; }

    void setQuantity(int quantity) { itemQuantity = quantity; }
    void setPrice(double price) { itemPrice = price; }

    void display() const {
        cout << "Category: " << itemCategory << "\n"
             << "ID: " << itemId << "\n"
             << "Item Name: " << itemName << "\n"
             << "Price: " << std::fixed << std::setprecision(2) << itemPrice << "\n"
             << "Quantity: " << itemQuantity << "\n";
    }
};

//class used to handle menu input
class InputHandler {
public:
    // Helper function to convert string to lowercase
    string toLowerCase(const string& str) const {
        string lowerStr = str;
        for (char& c : lowerStr) {
            c = tolower(c);
        }
        return lowerStr;
    }
    
     // Helper function to convert string to uppercase for displaying items
    string toUpperCase(const string& str) const {
        string upperStr = str;
        for (char& c : upperStr) {
            c = toupper(c);
        }
        return upperStr;
    }

    // Helper function to check if a string is a valid integer
    bool isValidInteger(const string& str) const {
        if (str.empty()) return false;  // Empty input is invalid
        for (char c : str) {
            if (!isdigit(c)) return false;  // Non-digit character found
        }
        return true;
    }

    // Helper function to check if a string is a valid double
    bool isValidDouble(const string& str) const {
        if (str.empty()) return false;  // Empty input is invalid
        bool decimalPointFound = false;
        for (char c : str) {
            if (c == '.') {
                if (decimalPointFound) return false;  // More than one decimal point found
                decimalPointFound = true;
            } else if (!isdigit(c)) {
                return false;  // Non-digit and non-decimal point character found
            }
        }
        return true;
    }

    // Generic function to get string input with the ability to cancel (case-insensitive)
    bool getInput(string prompt, string& input, const string& cancelKey = "C") const {
        cout << prompt;
        getline(cin, input);
        if (toLowerCase(input) == toLowerCase(cancelKey)) {
            cout << "\n> Action cancelled, going back to menu...\n";
            cout << "Press any key to continue...\n";
            cin.get();  // Wait for user input before returning
            system("cls");
            return false;  // Return false to indicate cancellation
        }
        return true;  // Return true if input is valid and not cancelled
    }

    // Function to get integer input with validation and cancellation
    bool getInput(string prompt, int& input, const string& cancelKey = "C") const {
        string strInput;
        cout << prompt;
        getline(cin, strInput);
        if (toLowerCase(strInput) == toLowerCase(cancelKey)) {
            cout << "\n> Action cancelled, going back to menu...\n";
            cout << "Press any key to continue...\n";
            cin.get();
            system("cls");
            return false;
        }
        if (!isValidInteger(strInput)) {
            cout << "\n> Invalid input, please enter a valid number.\n";
            return getInput(prompt, input, cancelKey);  // Retry on invalid input
        }
        input = stoi(strInput);
        return true;
    }

    // Function to get double input with validation and cancellation
    bool getInput(string prompt, double& input, const string& cancelKey = "C") const {
        string strInput;
        cout << prompt;
        getline(cin, strInput);
        if (toLowerCase(strInput) == toLowerCase(cancelKey)) {
            cout << "\n> Action cancelled, going back to menu...\n";
            cout << "Press any key to continue...\n";
            cin.get();
            return false;
        }
        if (!isValidDouble(strInput)) {
            cout << "\n> Invalid price, please enter a positive value (only up to 10 digits).\n";
            return getInput(prompt, input, cancelKey);  // Retry on invalid input
        }
        input = stod(strInput);
        return true;
    }
};
    
//abstract class used for validation of values
class AbstractValidation {
public:
    virtual bool validateId(const string& id) const = 0;
    virtual bool validateQuantity(int quantity) const = 0;
    virtual bool validatePrice(double price) const = 0;
};

//implementation of a concrete validation class
class ItemValidation : public AbstractValidation {
public:
    bool validateId(const string& id) const override {
        // Validation for ID (e.g., checking length, format)
        return !id.empty() && isValidIdOrCategory(id);
    }

    bool validateQuantity(int quantity) const override {
        return quantity > 0;
    }

    bool validatePrice(double price) const override {
        if (price <= 0.0) return false;

        // Convert the price to a string and check if the whole part has 10 or fewer digits
        stringstream priceStream;
        priceStream << fixed << setprecision(2) << price;
        string priceStr = priceStream.str();

        // Find the position of the decimal point
        size_t decimalPos = priceStr.find('.');
        string wholePart = (decimalPos != string::npos) ? priceStr.substr(0, decimalPos) : priceStr;

        return wholePart.length() <= 10;  // Ensure that the whole part has 10 or fewer digits
    }

	bool validateCategory(const string& category) const {
        string lowerCategory = toLowerCase(category);
        return lowerCategory == "clothing" || lowerCategory == "entertainment" || lowerCategory == "electronics";
    }

    bool isValidIdOrCategory(const string& str) const {
        for (char c : str) {
            if (!isalnum(c)) {  // Check for non-alphanumeric characters
                return false;
            }
        }
        return true;
    }

    bool isValidNumericInput(const string& input, double& output) const {
        for (char c : input) {
            if (!isdigit(c) && c != '.') {
                return false;  // Invalid character found
            }
        }
        output = stod(input);  // Convert valid string to double
        return true;
    }

    bool isValidNumericInput(const string& input, int& output) const {
        for (char c : input) {
            if (!isdigit(c)) {
                return false;  // Invalid character found
            }
        }
        output = stoi(input);  // Convert valid string to int
        return true;
    }
    
    // Helper function to convert a string to lowercase
    string toLowerCase(const string& str) const {
        string lowerStr = str;
        for (char& c : lowerStr) {
            c = tolower(c);
        }
        return lowerStr;
    }
};

// class used to handle adding items in the inventory
class AddItem {
private:
    vector<Item>& inventory;
    ItemValidation& validation;
    InputHandler inputHandler; 

public:
    AddItem(vector<Item>& inv, ItemValidation& val) : inventory(inv), validation(val) {}

    // Helper function to check for duplicate IDs in the inventory
    bool isDuplicateId(const string& id) const {
        for (const auto& item : inventory) {
            if (item.getId() == id) {
                return true;  // Duplicate ID found
            }
        }
        return false;  // No duplicate found
    }

	void addNewItem() {
	    string id, name, category;
	    int quantity;
	    double price;
	
	    cout << "===========================================\n";
	    cout << "\t\tADDING ITEM\n";
	    cout << "===========================================\n";
	    cout << "> Adding Item...\n";
	    cout << "> Input 'C' to cancel anytime.\n\n";
	
	    // Loop for category input and validation
	    while (true) {
	        if (!inputHandler.getInput("[Category]: ", category)) return;
	        
	        // Validate that category is one of the three allowed options
	        if (validation.validateCategory(category)) {
	            category = inputHandler.toLowerCase(category);  // Standardize the category as lowercase
	            break;
	        }
	        cout << "\n> Invalid category, please enter one of the following: clothing, entertainment, electronics.\n" << endl;
	    }
	
	    // Loop until a unique ID is entered
	    while (true) {
	        if (!inputHandler.getInput("[ID]: ", id)) return;
	
	        // Convert ID to uppercase
	        id = inputHandler.toUpperCase(id);
	
	        // Validate ID
	        if (!validation.validateId(id)) {
	            cout << "\n> Invalid ID, please enter a valid ID (alphanumeric characters only).\n";
	            continue;
	        }
	
	        // Check if the ID already exists in the inventory
	        if (!isDuplicateId(id)) {
	            break;  // Exit the loop if the ID is unique
	        } else {
	            cout << "\n> Error: An item with ID '" << id << "' already exists in the inventory.\n";
	        }
	    }
	
	    // Loop for item name input (no validation needed for name)
	    if (!inputHandler.getInput("[Item Name]: ", name)) return;
	
	    // Loop for price input and validation
	    while (true) {
	        if (!inputHandler.getInput("[Price]: ", price)) return;
	        if (validation.validatePrice(price)) break;
	        cout << "\n> Invalid price, please enter a positive value (only up to 10 digits).\n";
	    }
	
	    // Loop for quantity input and validation
	    while (true) {
	        if (!inputHandler.getInput("[Quantity]: ", quantity)) return;
	        if (validation.validateQuantity(quantity)) break;
	        cout << "\n> Invalid quantity, please enter a non-negative value.\n";
	    }
	
	    // If all validations pass, add the item to the inventory
	    Item newItem(id, name, quantity, price, category);
	    inventory.push_back(newItem);  // Add to inventory if no duplicates
	    cout << "\n> Item added successfully!\n";
	    newItem.display();
	    cout << " " << endl;
	
	    system("pause");
	    system("cls");
	}
};

//derived class for searching ID for managing items
class AbstractSearchByID {
protected:
    vector<Item>& inventory;

public:
    AbstractSearchByID(vector<Item>& inv) : inventory(inv) {}
    
    virtual void searchById(const string& id) = 0; 
};

//class for updating item derived from search by ID
class UpdateItem : public AbstractSearchByID {
private:
    AbstractValidation& validation;  // Validation object
    InputHandler inputHandler;
    Item* foundItem;  // Pointer to store the found item

public:
    UpdateItem(vector<Item>& inv, AbstractValidation& val)
        : AbstractSearchByID(inv), validation(val), foundItem(nullptr) {}

    // Function to search for the item by ID and store it internally
    void searchById(const string& id) override {
        // Check if the inventory is empty
        if (inventory.empty()) {
            cout << "> No items to update in inventory! Please add some items first.\n";
            system("pause");
            system("cls");
            return;
        }

        // Validate the ID
        if (!validation.validateId(id)) {
            cout << "> Invalid ID.\n";
            return;
        }

        // Search for the item in the inventory by ID
        for (auto& item : inventory) {
            if (item.getId() == id) {
                cout << "\n> Item found, updating the following item...\n\n";
                item.display();  // Display item details
                foundItem = &item;  // Store the reference to the found item
                return;  // Exit after finding the item
            }
        }
        cout << "> Item with ID " << id << " not found.\n";
        foundItem = nullptr;  // Set to nullptr if not found
    }

    // Header for update item display
    void updateItemHeader() {
        cout << "===========================================\n";
        cout << "\t\tUPDATE ITEM\n";
        cout << "===========================================\n";
    }

    // Main function for updating item
	void updateItem() {
	    string id, tryAgain;
	    bool cancelled = false;  // Flag to check if the user cancelled the operation
	
	    // Early exit if inventory is empty
	    if (inventory.empty()) {
	        updateItemHeader();
	        cout << "> No items to update in inventory! Please add some items first.\n";
	        system("pause");
	        system("cls");
	        return;
	    }
	
	    // Loop for updating items if the user wants to try again
	    do {
	        // Reset the cancel flag at the start of each iteration
	        cancelled = false;
	
	        // Loop for ID input and validation
	        while (true) {
	            system("cls");
	            updateItemHeader();
	            cout << "> Enter ID to update\n" << endl;
	            cout << "> Input 'C' to cancel anytime." << endl;
	            cout << "[ID]: ";  // Input prompt
	
	            // Get the ID input with cancellation option
	            if (!inputHandler.getInput("", id)) {
	                cancelled = true;  // Set cancelled flag if user cancels
	                break;  // Exit the ID input loop if cancelled
	            }
	
	            // Convert ID to uppercase before searching
	            id = inputHandler.toUpperCase(id);
	
	            // Call the search function with the converted uppercase ID
	            searchById(id);  // Search for the item
	
	            // Check if an item was found
	            if (foundItem) {
	                // Item found, break out of the ID input loop to proceed with update
	                break;
	            } else {                
	                system("pause");
	            }
	        }
	
	        // If the operation was cancelled during ID input, exit the entire process
	        if (cancelled) return;
	
	        // Loop for updating quantity or price
	        while (true) {
	            string input;
	            int newQuantity;
	            double newPrice;
	
	            cout << "\n1 - Update Quantity\n";
	            cout << "2 - Update Price\n";
	            cout << "> Input 'C' to cancel anytime.\n";
	
	            // Get the user's choice with cancellation option
	            if (!inputHandler.getInput("\n[Choice]: ", input)) {
	                cancelled = true;  // Set cancelled flag if user cancels
	                break;  // Exit the loop if cancelled
	            }
	
	            if (input == "1") {
	                // Update quantity with cancellation
	                cout << "\n> Enter new quantity." << endl;
	                if (!inputHandler.getInput("\n[Quantity]: ", newQuantity)) {
	                    cancelled = true;  // Set cancelled flag if user cancels
	                    break;  // Exit the loop if cancelled
	                }
	
	                if (validation.validateQuantity(newQuantity)) {
	                    foundItem->setQuantity(newQuantity);
	                    cout << "\n> Quantity updated successfully.\n";
	                } else {
	                    cout << "> Quantity update failed due to invalid input.\n";
	                }
	                break;  // Exit the loop after a successful update
	            } else if (input == "2") {
	                // Update price with cancellation
	                cout << "\n> Enter new price." << endl;
	                if (!inputHandler.getInput("\n[Price]: ", newPrice)) {
	                    cancelled = true;  // Set cancelled flag if user cancels
	                    system("cls");      // Clear the screen
	                    break;              // Exit the loop if cancelled
	                }
	
	                if (validation.validatePrice(newPrice)) {
	                    foundItem->setPrice(newPrice);
	                    cout << "\n> Price updated successfully.\n";
	                } else {
	                    cout << "> Price update failed due to invalid input.\n";
	                }
	                break;  // Exit the loop after a successful update
	            } else {
	                cout << "> Invalid choice. Please select 1 or 2.\n";  // Stay in the loop for invalid input
	            }
	        }
	
	        // Only display the updated item if the operation was not cancelled
	        if (!cancelled) {
	            foundItem->display();  // Display updated item only once after the changes
	        }
	
	        // Ask user if they want to try updating another item
	        while (true) {
	            if (!inputHandler.getInput("\n> Update another item? [Y/N]: ", tryAgain)) return;
	            tryAgain = inputHandler.toUpperCase(tryAgain);
	            if (tryAgain == "Y" || tryAgain == "N") break;
	            cout << "> Invalid input. Please enter 'Y' or 'N'.\n";
	        }
	
	    } while (tryAgain == "Y");
	
	    cout << "> Exiting update item process.\n";
	    system("pause");
	    system("cls");
	}
};

// Class used to handle removing items from the inventory
class RemoveItem : public AbstractSearchByID {
private:
    InputHandler inputHandler;

public:
    // Constructor for RemoveItem, passing inventory to the base class constructor
    RemoveItem(vector<Item>& inv) : AbstractSearchByID(inv) {}

    void searchById(const string& id) override {
        // Convert the input ID to lowercase for a case-insensitive search
        string lowerId = inputHandler.toLowerCase(id);
        char confirm;  // Variable to hold the user's confirmation input

        for (size_t i = 0; i < inventory.size(); ++i) {
            if (inputHandler.toLowerCase(inventory[i].getId()) == lowerId) {
                cout << "\n> Item found:\n";
                inventory[i].display();  // Display the found item

                // Ask for confirmation before removing the item
                while (true) {
                    cout << "\n> Confirm to delete item?" << endl;
                    cout << "[Y/N]: ";
                    cin >> confirm;
                    confirm = tolower(confirm);  // Convert to lowercase for easy comparison

                    if (confirm == 'y') {
                        // User confirmed, remove the item
                        cout << "\n> Removing item...\n";
                        inventory.erase(inventory.begin() + i);
                        cout << "\n> Item removed successfully.\n";
                        return;  // Exit after removing the item
                    } else if (confirm == 'n') {
                        // User cancelled, do not remove the item, return to the menu
                        cout << "\n> Item removal cancelled.\n";
                        return;
                    } else {
                        // Invalid input, prompt again
                        cout << "\n> Invalid input, please enter 'Y' or 'N'.\n";
                    }
                }
            }
        }
        // If no item is found, display a message
        cout << "> Item with ID " << id << " not found.\n";
    }

	void removeItemHeader(){
		cout << "===========================================\n";
	    cout << "\t\tREMOVE ITEM\n";
	    cout << "===========================================\n";
	}

	void removeItem() {
	    string id;
	    char retry;
	
	    if (inventory.empty()) {
	    	removeItemHeader();
	        cout << "> No items to remove in inventory! Please add some items first.\n";
	        system("pause");
	        system("cls");
	        return;
	    }
	
	    do {
	    	system("cls");
	    	removeItemHeader();
	        cout << "> Enter ID to remove: ";
	        getline(cin, id);
	
	        // Check if the ID is empty
	        if (id.empty()) {
	            cout << "> Invalid input! Please enter a valid ID.\n";
	            continue;  // Go to the next iteration to prompt again
	        }
	
	        searchById(id);  // Call search and removal process
	
	        // If the item was removed, ask the user if they want to remove another
	        while (true) {
	            cout << "\n> Remove another item? [Y/N]: ";
	            cin >> retry;
	            cin.ignore();
	            retry = tolower(retry);  // Convert to lowercase for easy comparison
	
	            if (retry == 'y' || retry == 'n') {
	                break;  // Exit the inner loop if the input is valid
	            } else {
	                cout << "\n> Invalid input, please enter 'Y' or 'N'.\n";
	            }
	        }
	
	    } while (retry == 'y');  // Continue if user chooses to try again ('Y')
	
	    system("cls");  // Clear screen before returning to the main menu
	}
};

// class used to search items in the inventory
class SearchItem : public AbstractSearchByID {
private:
    InputHandler inputHandler;

public:
    SearchItem(vector<Item>& inv) : AbstractSearchByID(inv) {}

    void searchById(const string& id) override {
        for (const auto& item : inventory) {
            if (item.getId() == id) {
                cout << "> Item found!\n" << endl;
                item.display();
                return;  // Exit after displaying the item
            }
        }
        cout << "> Item with ID " << id << " not found.\n";
    }

	void searchItemHeader(){
        cout << "===========================================\n";
        cout << "\t\tSEARCH ITEM\n";
        cout << "===========================================\n";
	}

    void searchItem() {
        string id;
        char retry;

        if (inventory.empty()) {
        	searchItemHeader();
            cout << "> No items to search in inventory! Please add some items first.\n";
            system("pause");
            system("cls");
            return;
        }

        // Loop for searching items
        do {
        	system("cls");
        	searchItemHeader();
        	
            cout << "> Enter ID to search: ";
            cin >> id;

            // Convert ID to uppercase before searching
            id = inputHandler.toUpperCase(id);

            searchById(id);

            // Ask user if they want to search another item
            while (true) {
                cout << "\n> Search another item? [Y/N]: ";
                cin >> retry;
                retry = tolower(retry);  // Convert to lowercase for easy comparison

                if (retry == 'y' || retry == 'n') {
                    break;  // Exit the loop if input is valid
                } else {
                    cout << "\n> Invalid input, please enter 'Y' or 'N'.\n";
                }
            }

        } while (retry == 'y');  // Continue if user chooses to search again ('Y')

        system("cls");  // Clear the screen before returning to the main menu
    }
};

//Abstract class used to display the whole inventory
class DisplayAllItems {
protected:
    vector<Item>& inventory;  // Reference to the inventory

    virtual void displayTableHeader() const {
        int lineWidth = 65;  // Width of the separator line (the length of "===========================================")
        string title = "INVENTORY";

        cout << string(lineWidth, '=') << "\n";  // Print top separator line
        cout << setw((lineWidth + title.length()) / 2) << title << "\n";  // Center the title
        cout << string(lineWidth, '=') << "\n";  // Print bottom separator line

        // Set column headers with specific widths for clean alignment
        cout << left << setw(15) << "CATEGORY"
             << left << setw(10) << "ID"
             << left << setw(20) << "NAME"
             << right << setw(10) << "QUANTITY"
             << right << setw(10) << "PRICE\n";
        cout << "-----------------------------------------------------------------\n";
    }

    // Helper function to display a single item in the table
    void displayItem(const Item& item) const {
        // Display each item with proper alignment
        cout << left << setw(15) << item.getCategory()
             << left << setw(10) << item.getId()
             << left << setw(20) << item.getName()
             << right << setw(10) << item.getQuantity()
             << right << setw(10) << item.getPrice() << "\n";
    }

public:
    DisplayAllItems(vector<Item>& inv) : inventory(inv) {}

    // Pure virtual function to be implemented by derived classes
    virtual void displayItems() const = 0;
};

// class used to display the whole inventory
class DisplayInventory : public DisplayAllItems {
public:
    DisplayInventory(vector<Item>& inv) : DisplayAllItems(inv) {}
	
    void displayItems() const override {
        if (inventory.empty()) {
			int lineWidth = 65;  // Width of the separator line (the length of "===========================================")
		        string title = "INVENTORY";
		
		    cout << string(lineWidth, '=') << "\n";  // Print top separator line
		    cout << setw((lineWidth + title.length()) / 2) << title << "\n";  // Center the title
		    cout << string(lineWidth, '=') << "\n";  // Print bottom separator line
            cout << "> No items to display in inventory! Please add some items first.\n";
            system("pause");
            system("cls");
            return;
        }

        // Display header and items
        displayTableHeader();
        for (const auto& item : inventory) {
            displayItem(item);
        }
        
        system("pause");
        system("cls");
    }
};

// Class used to display the items by category
class DisplayCategoryItems : public DisplayAllItems {
public:
    DisplayCategoryItems(vector<Item>& inv) : DisplayAllItems(inv) {}

    // Override to provide a custom header for category items
    void displayTableHeader() const override {
        int lineWidth = 65;  // Width of the separator line (the length of "===========================================")
        string title = "ITEMS BY CATEGORY";

        cout << string(lineWidth, '=') << "\n";  // Print top separator line
        cout << setw((lineWidth + title.length()) / 2) << title << "\n";  // Center the title
        cout << string(lineWidth, '=') << "\n";  // Print bottom separator line

    }

    // Convert string to lowercase (helper function)
    string toLower(const string& str) const {
        string lowerStr;
        for (char c : str) {
            lowerStr += (c >= 'A' && c <= 'Z') ? c + 32 : c;  // Convert to lowercase
        }
        return lowerStr;
    }

    // Override displayItems to display items by category
    void displayItems() const override {

        // Check if the inventory is empty and display a message if so
        if (inventory.empty()) {
            displayTableHeader();
            cout << "> No items to display in inventory! Please add some items first.\n";
            system("pause");
            system("cls");
            return;  // Exit the function early if there are no items
        }

        string tryAgain;

        // Loop for displaying items by category
        do {
            int choice;
            string selectedCategory;
			system("cls");
            // Display category options
        	displayTableHeader();
            cout << "> Select category:\n";
            cout << "1 - Clothing\n";
            cout << "2 - Electronics\n";
            cout << "3 - Entertainment\n";
            cout << "[CHOICE]: ";
            cin >> choice;

            // Determine selected category based on user input
            switch (choice) {
                case 1: selectedCategory = "Clothing"; break;
                case 2: selectedCategory = "Electronics"; break;
                case 3: selectedCategory = "Entertainment"; break;
                default:
                    cout << "> Invalid choice!\n";
                    system("pause");
                    system("cls");
                    continue;  // Continue the loop if the choice is invalid
            }

            string lowerSelectedCategory = toLower(selectedCategory);
			system("cls");
            displayTableHeader();
            // Set column headers with specific widths for clean alignment
       		 cout << left << setw(15) << "CATEGORY"
             << left << setw(10) << "ID"
             << left << setw(20) << "NAME"
             << right << setw(10) << "QUANTITY"
             << right << setw(10) << "PRICE\n";
       		 cout << "-----------------------------------------------------------------\n";
            bool found = false;

            // Search and display items in the selected category
            for (const auto& item : inventory) {
                if (toLower(item.getCategory()) == lowerSelectedCategory) {
                    displayItem(item);  // inherited helper function to display the item
                    found = true;
                }
            }

            if (!found) {
                cout << "> No items found in the " << selectedCategory << " category.\n";
            }

            // Prompt the user to view another category
            while (true) {
                cout << "\n> View another category? [Y/N]: ";
                cin >> tryAgain;
                tryAgain = toLower(tryAgain);  // Convert user input to lowercase for comparison
                if (tryAgain == "y" || tryAgain == "n") break;
                cout << "> Invalid input. Please enter 'Y' or 'N'.\n";
            }  
		
        } while (tryAgain == "y");

        cout << "> Exiting category view.\n";
        system("cls");
    }
};

// class used to handle sorting and display sorted inventory
class SortItems : public DisplayAllItems {
public:
    SortItems(vector<Item>& inv) : DisplayAllItems(inv) {}

    // Override to provide a custom header for sorted items
    void displayTableHeader() const override {
        int lineWidth = 65;  // Width of the separator line (the length of "===========================================")
        string title = "SORTED ITEMS";

        cout << string(lineWidth, '=') << "\n";  // Print top separator line
        cout << setw((lineWidth + title.length()) / 2) << title << "\n";  // Center the title
        cout << string(lineWidth, '=') << "\n";  // Print bottom separator line
    }

    // Override displayItems to display sorted items based on user choice
    void displayItems() const override {
        int sortBy = 0, sortOrder = 0;
        string input;
        char retry;  // Variable to ask if the user wants to sort again
        InputHandler inputHandler;  // Use InputHandler for managing input with cancel functionality
        ItemValidation validator;

        do {
            displayTableHeader();

            // Check if there are 1 or fewer items in inventory
            if (inventory.size() <= 1) {
                cout << "> Not enough items to sort! Please ensure you have more than 1 item.\n";
                system("pause");
                system("cls");
                return;
            }

            // Validate choice for sorting by price or quantity, with cancel option
            while (true) {
                if (!inputHandler.getInput("> How would you like to sort the items?\n1 - Price\n2 - Quantity\n\n[CHOICE]: ", input)) return;
                if (validator.isValidNumericInput(input, sortBy) && (sortBy == 1 || sortBy == 2)) break;
                cout << "\n> Invalid choice! Please enter 1 or 2.\n";
            }

            // Validate choice for sorting order (ascending or descending), with cancel option
            while (true) {
                if (!inputHandler.getInput("\n> Sort in which order?\n1 - Ascending\n2 - Descending\n\n[CHOICE]: ", input)) return;
                if (validator.isValidNumericInput(input, sortOrder) && (sortOrder == 1 || sortOrder == 2)) break;
                cout << "\n> Invalid choice! Please enter 1 or 2.\n";
            }

            // Sorting logic using Insertion Sort
            vector<Item> sortedInventory = inventory;
            insertionSort(sortedInventory, sortBy, sortOrder);

            // Call the inherited display method to display the sorted items
            system("cls");
            displayTableHeader();
	        // column headers with specific widths for clean alignment
	        cout << left << setw(15) << "CATEGORY"
	             << left << setw(10) << "ID"
	             << left << setw(20) << "NAME"
	             << right << setw(10) << "QUANTITY"
	             << right << setw(10) << "PRICE\n";
       		 cout << "-----------------------------------------------------------------\n";	
            for (const auto& item : sortedInventory) {
                displayItem(item);  // Use the inherited helper function to display the item
            }

			// Ask the user if they want to sort again
			while (true) {  // Loop until valid input is given
			    cout << "> Would you like to sort again? [Y/N]: ";
			    cin >> retry;
			    retry = tolower(retry);  // Convert to lowercase for comparison
			
			    // Clear the input buffer by discarding the newline character
			    cin.ignore(1, '\n'); 
			
			    if (retry == 'y' || retry == 'n') {
			        break;  // Exit the loop if valid input
			    } else {
			        cout << "\n> Invalid input! Please enter 'Y' or 'N'.\n";
			    }
			}

        system("cls");
        } while (retry == 'y');  // Loop as long as the user wants to sort again
    }

private:
    // Insertion Sort algorithm for sorting the inventory based on user choices
    void insertionSort(vector<Item>& items, int sortBy, int sortOrder) const {
        int n = items.size();
        for (int i = 1; i < n; i++) {
            Item key = items[i];
            int j = i - 1;

            // Sort by price
            if (sortBy == 1) {
                while (j >= 0 && (sortOrder == 1 ? items[j].getPrice() > key.getPrice() : items[j].getPrice() < key.getPrice())) {
                    items[j + 1] = items[j];
                    j--;
                }
            }
            // Sort by quantity
            else if (sortBy == 2) {
                while (j >= 0 && (sortOrder == 1 ? items[j].getQuantity() > key.getQuantity() : items[j].getQuantity() < key.getQuantity())) {
                    items[j + 1] = items[j];
                    j--;
                }
            }

            items[j + 1] = key; // Place the key in its correct position
        }
    }
};

// Class used to display items that are low in stock
class DisplayLowStock {
private:
    vector<Item>& inventory; // Reference to the inventory

public:
    // Constructor
    DisplayLowStock(vector<Item>& inv) : inventory(inv) {}

    // Function to display the header
    void displayHeader() {
        int lineWidth = 65;  // Width of the separator line (the length of "===========================================")
        string title = "MONITORING LOW STOCK";

        cout << string(lineWidth, '=') << "\n";  // Print top separator line
        cout << setw((lineWidth + title.length()) / 2) << title << "\n";  // Center the title
        cout << string(lineWidth, '=') << "\n";  // Print bottom separator line
    }

    // Function to display items that are low in stock
    void displayLowStockItems() {
        // Display the header at the beginning
        displayHeader();

        // Check if the inventory is empty
        if (inventory.empty()) {
            cout << "> No items to display in inventory! Please add some items first.\n";
            system("pause");
            system("cls");
            return;
        }

        bool foundLowStock = false; 

        // Column headers with specific widths for clean alignment
        cout << left << setw(15) << "CATEGORY"
             << left << setw(10) << "ID"
             << left << setw(20) << "NAME"
             << right << setw(10) << "QUANTITY"
             << right << setw(10) << "PRICE\n";
        cout << "-----------------------------------------------------------------\n";

        // Iterate through the inventory to find low stock items
        for (const auto& item : inventory) {
            if (item.getQuantity() <= 5) {
                // Display items that have quantity <= 5 using aligned columns
                cout << left << setw(15) << item.getCategory()
                     << left << setw(10) << item.getId()
                     << left << setw(20) << item.getName()
                     << right << setw(10) << item.getQuantity()
                     << right << setw(10) << fixed << setprecision(2) << item.getPrice() << "\n";
                foundLowStock = true;
            }
        }

        // If no low-stock items were found, display a message
        if (!foundLowStock) {
            cout << "\n> No items are currently low in stock.\n";
        }

        system("pause");
        system("cls");
    }
};

// class used for handling menus and user interaction
class DisplayMenu {
private:
    vector<Item> inventory;
    AddItem addItem;
    ItemValidation validation;

public:
    DisplayMenu() : addItem(inventory, validation) {}  // Pass validation to AddItem

    void showMenu() {
        int choice;
        string input;

        do {
            // Display the menu
            cout << "===========================================\n";
            cout << "\t\tMENU\n";
            cout << "===========================================\n";
            cout << "1 - Add Item\n";
            cout << "2 - Update Item\n";
            cout << "3 - Remove Item\n";
            cout << "4 - Display Items by Category\n";
            cout << "5 - Display All Items\n";
            cout << "6 - Search Item\n";
            cout << "7 - Sort Items\n";
            cout << "8 - Display Low Stock Items\n";
            cout << "9 - Exit\n";
            
            // Loop to get valid input from the user
            do {
                cout << "\n> Please make a choice\n";
                cout << "[CHOICE]: ";
                cin >> input;
                
            // Clear the input buffer
            cin.ignore();  // Discards the newline character left in the input buffer

                // Validate if the input is numeric and within the valid range
                if (input.length() == 1 && isdigit(input[0])) {
                    choice = stoi(input);  // Convert to integer if valid
                } else {
                    choice = 0;  // Set to invalid choice if input is not valid
                }

                if (choice < 1 || choice > 9) {
                    cout << "\n> Invalid choice! Please enter a number between 1 and 9.\n";
                }

            } while (choice < 1 || choice > 9);  // Keep asking until valid input is given

            // Process the valid choice
            switch (choice) {
                case 1:
                    system("cls");
                    addItem.addNewItem();
                    break;
                case 2: {
                    system("cls");
                    UpdateItem update(inventory, validation);
                    update.updateItem();
                    break;
                }
                case 3: {
                    system("cls");
                    RemoveItem remove(inventory);
                    remove.removeItem();
                    break;
                }
                case 4: {  // Display items by category
                    system("cls");
                    DisplayCategoryItems displayCategory(inventory); // Pass the shared inventory
                    displayCategory.displayItems(); // Call displayItems to show category items
                    break;
                }
                case 5: { // Display all items
                    system("cls");
                    DisplayInventory displayInventory(inventory);  // Use the derived class
                    displayInventory.displayItems(); // Call displayItems
                    break;
                }
                case 6: {
                    system("cls");
                    SearchItem search(inventory);
                    search.searchItem();
                    break;
                }
                case 7: { // Sort items
                    system("cls");
                    SortItems sortItems(inventory); // Pass the shared inventory
                    sortItems.displayItems(); // Call displayItems to sort and display
                    break;
                }
                case 8: {
                    system("cls");
                    DisplayLowStock displayLowStock(inventory);  // Pass the shared inventory
                    displayLowStock.displayLowStockItems();
                    break;
                }
                case 9:
                    cout << "Exiting...\n";
                    break;
            }

        } while (choice != 9);
    }
};

// main function
int main() {
    DisplayMenu menu;
    menu.showMenu();
    return 0;
}
