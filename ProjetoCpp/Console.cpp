#include "Console.h"

#include <iostream>
#include <stdlib.h>
#include <ctype.h>

using namespace std;

Console::Console(): platform(" "), softwareVersion(1.0), releaseDate(1,1,1900), lastUpdated(1,1,1900), internetConnection(false), nGames(0.00), nUsers(0.00)
{
    for (int i=0 ; i < CONTROLLER_SLOTS; i++)
    {
        controllers[i].setConnected(false);
        controllers[i].setWireless(false);
        controllers[i].setPosXYZ(0,0,0);
    }
   
   for (int i=0; i < 3; i++)
       storage[i] = 0.00;
}

Console::Console(const Console &c)
{  

    power = c.power;
    manufacturer = c.manufacturer;
    intStorage = c.intStorage;
    for (int i = 0; i < USB_PORTS; i++)
        extStorage[i] = c.extStorage[i];
    ethernetCard = c.ethernetCard;
  
    platform = c.platform;
    softwareVersion = c.softwareVersion;
    releaseDate = c.releaseDate;
    lastUpdated = c.lastUpdated;
    storage[0] = c.storage[0]; //Espaço total
    storage[1] = c.storage[1]; //Espaço usado
    storage[2] = c.storage[2]; //Espaço livre
    internetConnection = c.internetConnection;
    nGames = c.nGames;
    nUsers = c.nUsers;
    
    for (int i=0 ; i < CONTROLLER_SLOTS; i++)
        controllers[i] = c.controllers[i];
    for (int i = 0; i < nGames; i++)
        games[i] = c.games[i];
    for (int i = 0; i < nUsers; i++)
        users[i] = c.users[i];
}

Console::~Console()
{       
    delete[] games;
    delete[] users;
}

void Console::connectToInternet()
{
    if (ethernetCard)
        if (!internetConnection)
            internetConnection = true;
}

void Console::refreshTotalSpace()
{
    storage[0] = intStorage;
    for (int i = 0; i < USB_PORTS; i++)
        storage[0] += extStorage[i];
}

void Console::refreshUsedSpace(float sz)
{
    storage[1] = storage[1] + sz;
}

void Console::refreshFreeSpace()
{
    storage[2] = storage[0] - storage[1];
}

int Console::findGame(string nm, int N)
{
     for (int i = 0; i < N; i++)
    { 
        if (nm.compare(games[i].getTitle()) == 0)
            return i;
    }
    return -1;
}

int Console::findUser(string nm, int N)
{
    for (int i = 0; i < N; i++)
    {
        if (nm.compare (users[i].getName()) == 0)
        return i;
    }
    return -1;
}

void Console::installGame(const Game& game)
{
    if (power)
    { 
        if (platform.compare(game.getPlatform()) == 0)
        {
            if (storage[2] > game.getSize())
            {
                if(nGames != 0)
                {
                    Game* aux = new Game[nGames];
                    
                    for(int i = 0; i < nGames; i++)
                        aux[i] = games[i];
                    
                    delete [ ] games;
                    games = new Game[++nGames];
            
                    for(int i = 0; i < nGames-1; i++)
                        games[i] = aux[i];
                    
                    games[nGames - 1] = game;
                    delete [ ] aux;
                }
                else
                {
                    storage[1] = game.getSize();
                    games = new Game[++nGames];
                    games[0] = game;
                }
                float sz = game.getSize();
                cout <<"\nThe game '"<< game.getTitle() <<"' was successfully installed on your system!";
                refreshUsedSpace(sz);
            }
            else
                cout <<"\nThe game '"<< game.getTitle() << "' could not be installed. Not enough storage space. ";
        }
        else
            cout <<"\nThe game '"<< game.getTitle() << "' could not be installed. It is not compatible with your system. ";
    }
    else
        cout << "\nThe game '"<< game.getTitle() << "' could not be installed. The device is turned off. ";
        
}

void Console::uninstallGame()
{
    string nm;
    cout << "\nWhich game would to like to uninstall? ";
    cin >> nm;
    int a = findGame(nm, nGames);
    
    if (a == -1)
	cout <<"\nThe game '" << nm  << "' was not found.";
   else
   {
    for (int i = a + 1; i < nGames; i++)
		games[i - 1] = games[i];
			
	nGames--;
    float sz = games[a].getSize();
    sz *= -1;
    refreshUsedSpace(sz);
	cout << "\nThe game '"<< nm << "' was uninstalled successfully!";
   }
}

void Console::createUser(const User &user)
{
    if (power)
    {
         if(nUsers != 0)
            {
                User* aux = new User[nUsers];
                
                for(int i = 0; i < nUsers; i++)
                    aux[i] = users[i];
                
                delete [ ] users;
                
                users = new User[++nUsers];
                
                for(int i = 0; i < nUsers-1; i++)
                    users[i] = aux[i];
                    
                users[nUsers - 1] = user;
                    
                delete [ ] aux;
            }
            else
            {
                users = new User[++nUsers];
                users[0] = user;
            }
            cout <<"\nWelcome, "<< users[nUsers-1].getGamertag() <<"!";
    }
    else
        cout << "\nUser could not be created. The device is turned off.";
}

void Console::deleteUser()
{
    string nm;
    cout << "\nWhich user would to like to delete? ";
    cin >> nm;
    int a = findUser(nm, nUsers);
    
    if (a == -1)
	cout <<"\nUser '" << nm  << "'could not be found.";
   else
   {
	for (int i = a + 1; i < nUsers; i++)
		users[i - 1] = users[i];
			
	nUsers--;
	cout << "\nUser '"<< nm << "' was deleted successfully!";
   }
}


void Console::insertController(unsigned short int slot)
{
    if ( slot >= 0 && slot < CONTROLLER_SLOTS )
        if (!(controllers[slot].getConnected()))
        {
            controllers[slot].setConnected(true);
            controllers[slot].setWireless(false);
            controllers[slot].setPosXYZ(0,0,0);
            cout <<"\nController " << slot << " connected.";
        }
        else
            cout <<"\nError. The specified port is in use.";
    else
        cout <<"\nError. The specified port does not exist.";
}

void Console::removeController(unsigned short int slot)
{
    if (slot >= 0 && slot < CONTROLLER_SLOTS)
    {
            controllers[slot].setConnected(false);
            controllers[slot].setWireless(false);
            controllers[slot].setPosXYZ(0,0,0);
    }
    else
        cout <<"\nError. The specified port does not exist.";
}

void Console::displayGames() const
{
    if ( nGames > 0)
    {
        cout << "\n\t-Installed Games-";
        for(int i = 0; i < nGames; i++)
        {
            games[i].gameInfo();
            cout <<"---";
        }
    }
     else
         cout << "\nThere are no games installed.";
}

void Console::displayUsers() const
{
    if ( nUsers > 0)
    {
        cout << "\n\t-Users-";
        for(int i = 0; i < nUsers; i++)
        {
            cout <<"\nName: " << users[i].getName();
            cout <<"\nGamertag: " << users[i].getName();
            cout <<"---";
        }
    }
     else
         cout << "\nThere are no users.";
}

void Console::update()
{
    char op;
    float latest = 0.15;
    connectToInternet();
    if (!internetConnection)
    {
        cout <<"\nInternet connection failed.";
        return;
    }
   
    if ((rand()%2) == 1)    
    {
        cout <<"\nThere are new updates available. Update system software? (Y/N)";
        cin >> op;
            if (toupper(op) == 'Y')
            {
                softwareVersion += latest;
                lastUpdated.timeNow();
            }
    }
    else
        cout <<"\nThere are no updates available.";      
}

void Console::consoleInfo() const
{
    deviceInfo();
    cout <<"\n\t-System Info-";
    
    if (power)
    {
        
        cout <<"\nPlatform: "<< platform;
        cout <<"\nSoftware version: " << softwareVersion;
        cout <<"\nTotal Space: "<< storage[0];
        cout <<"\nUsed Space: "<< storage[1];
        cout <<"\nFree Space: "<< storage[2];
        cout <<"\nInternet access: " << boolalpha << internetConnection;
        cout <<"\nNumber of games installed: "<< nGames;
        cout <<"\nNumber of users: "<< nUsers;
    }
    else
        cout <<"\nThe console is turned off.";
}


void Console::play() 
{
    string title;
    char op;

    cout << "\nType in the game you would like to play: ";
    cin >> title;
    
    int a = findGame(title, nGames);
        if (a == -1)
            cout << "\nThe game " << title << " is not installed on your system.";  
        else
        {
            cout << "\nPlay " << games[a].getTitle() << "(Y/N)?";
            cin  >> op;
            
            if (toupper(op) == 'Y')
            {
                cout <<"\nLoading game. Please wait...";
                cout << "\nYou're playing "<< games[a].getTitle() << "!\n";
            }
            else 
                cout <<"\nExiting...";
        }
}
//
ostream &operator<<(ostream &out, const Console &cons)
{
    out <<"\n\t-System Info-";
   
    if (cons.power)
    {
        out <<"\nPlatform: "<< cons.platform;
        out <<"\nSoftware version: " << cons.softwareVersion;
        out <<"\nTotal Space: "<< cons.storage[0];
        out <<"\nUsed Space: "<< cons.storage[1];
        out <<"\nFree Space: "<< cons.storage[2];
        out <<"\nInternet access: " << boolalpha << cons.internetConnection;
        out <<"\nNumber of games installed: "<< cons.nGames;
        out <<"\nNumber of users: "<< cons.nUsers;
    }
    else
        out <<"\nThe console is turned off.";
        
    return out;
}

const Console &Console::operator=(const Console &cons)
{
    power = cons.power;
    manufacturer = cons.manufacturer;
    intStorage = cons.intStorage;
    for (int i = 0; i < USB_PORTS; i++)
        extStorage[i] = cons.extStorage[i];
    ethernetCard = cons.ethernetCard;
    
    platform = cons.platform;
    softwareVersion = cons.softwareVersion;
    releaseDate = cons.releaseDate;
    lastUpdated = cons.lastUpdated;
    for (int i = 0; i < 3; i++)
        storage[i] = cons.storage[i];
    internetConnection = cons.internetConnection;
    nGames = cons.nGames;
    nUsers = cons.nUsers;
    
    delete [] users;
    delete [] games;
    
    users = new User[nUsers];
    games = new Game[nGames];
    
    for (int i=0; i < nUsers; i++)
    {
        users[i] = cons.users[i];
    }
    for (int i=0; i < nGames; i++)
    {
        games[i] = cons.games[i];
    }
    for (int i=0; i < CONTROLLER_SLOTS; i++)
    {
        controllers[i] = cons.controllers[i];
    }
    
    return *this;
}

bool Console::operator==(const Console &cons) const
{
   
    if (power != cons.power)
        return false;
    if (manufacturer != cons.manufacturer)
        return false;
    if (intStorage != cons.intStorage)
        return false;
    if (ethernetCard != cons.ethernetCard)
        return false;

    if(platform != cons.platform)
        return false;
    if(softwareVersion != cons.softwareVersion)
        return false;
    if(internetConnection != cons.internetConnection)
        return false;
    if(nUsers != cons.nUsers)
        return false;
    if(nGames != cons.nGames)
        return false;
          
    return true;
}