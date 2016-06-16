// MyApp.cpp
//
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include "wx/wx.h"
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;

// Declare the application class
class MyApp : public wxApp
{
public:
	// Called on application startup
	virtual bool OnInit();
};

// Declare our main frame class
class MyFrame : public wxFrame
{
public:
	// Constructor
	MyFrame(const wxString& title);
	// Event handlers
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnHelpSearch(wxCommandEvent& event);

	string HttpTest();
	string MySQLTest();

private:
	// This class handles events
	DECLARE_EVENT_TABLE()
};

// Implements MyApp& GetApp()
DECLARE_APP(MyApp)
// Give wxWidgets the means to create a MyApp object
IMPLEMENT_APP(MyApp)

// Initialize the application
bool MyApp::OnInit()
{
	// Create the main application window
	MyFrame *frame = new MyFrame(wxT("Minimal wxWidgets App"));
	// Show it
	frame->Show(true);
	// Start the event loop
	return true;
}

// Event table for MyFrame
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
EVT_MENU(wxID_HELP_SEARCH, MyFrame::OnHelpSearch)
EVT_MENU(wxID_EXIT, MyFrame::OnQuit)
END_EVENT_TABLE()

void MyFrame::OnAbout(wxCommandEvent& event)
{
	//wxString msg;
	//msg.Printf(wxT("Hello and welcome to %s"),
	//	wxVERSION_STRING);
	//wxMessageBox(msg, wxT("About Minimal"),
	//	wxOK | wxICON_INFORMATION, this);
	string msg = HttpTest();
	wxMessageBox(msg, wxT("HttpTest"), wxOK | wxICON_INFORMATION, this);
}

string MyFrame::HttpTest()
{
	string ip = "10.1.4.134";
	string http = "http";

	using namespace boost::asio;
	ip::tcp::iostream stream(ip, http);
	if (!stream)
	{
		return "";
	}
	stream << "GET / HTTP/1.0\r\n"
		<< "Host: " << ip << "\r\n"
		<< "\r\n";
	string response_line;
	while (true)
	{
		bool b = getline(stream, response_line).good();
		if (response_line.compare("\r") == 0)
		{
			b = getline(stream, response_line).good();
		}
		if (!b)
			break;
	};
	return response_line;
}

void MyFrame::OnHelpSearch(wxCommandEvent& event)
{
	string msg = MySQLTest();
	wxMessageBox(msg, wxT("MySQLTest"), wxOK | wxICON_INFORMATION, this);
}

string MyFrame::MySQLTest()
{
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *res;

		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "1qaz2wsX=");
		con->setSchema("world");

		stmt = con->createStatement();
		res = stmt->executeQuery("select Name from city where CountryCode = 'TWN'");
		//res = stmt->executeQuery("select count(*) from city");

		string result;
		while (res->next()) {
			result = res->getString("Name").c_str();
			//int count = res->getInt(1);
			break;
		}
		delete res;
		delete stmt;
		delete con;
		return result;
	}
	catch (sql::SQLException &e)
	{
		return "SQLException";
	}
}

void MyFrame::OnQuit(wxCommandEvent& event)
{
	// Destroy the frame
	Close();
}

//#include "mondrian.xpm"

MyFrame::MyFrame(const wxString& title)
: wxFrame(NULL, wxID_ANY, title)
{
	// Set the frame icon
	//SetIcon(wxIcon(mondrian_xpm));
	// Create a menu bar
	wxMenu *fileMenu = new wxMenu;
	// The "About" item should be in the help menu
	wxMenu *helpMenu = new wxMenu;
//	helpMenu->Append(wxID_ABOUT, wxT("&About...\tF1"),
//		wxT("Show about dialog"));
	helpMenu->Append(wxID_ABOUT, wxT("&HttpTest"),
		wxT("Http test"));
	helpMenu->Append(wxID_HELP_SEARCH, wxT("&MySQLTest"),
		wxT("MySQL test"));
	fileMenu->Append(wxID_EXIT, wxT("E&xit\tAlt - X"),
		wxT("Quit this program"));
	// Now append the freshly created menu to the menu bar...
	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, wxT("&File"));
	menuBar->Append(helpMenu, wxT("&Help"));
	// ... and attach this menu bar to the frame
	SetMenuBar(menuBar);
	// Create a status bar just for fun
	CreateStatusBar(2);
	SetStatusText(wxT("Welcome to wxWidgets!"));
}
