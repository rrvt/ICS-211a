# ICS_211a

Read San Jose Radio Amateur Civil Emergency Service badge barcodes to facilitate check-in and check-out
at events.  Also allows members without badges and visitors to check-in and check-out.  Allows
check-outs to be performed by someone after everyone is gone.  Prepares Excel spreadsheet reports.

## Features

The idea of the program is that a laptop and barcode reader are right next to the door.  Members and
non-members enter the meeting room.  Members with their badge wave it in front of the barcode reader.
Members without their badge enter the information via a dialog box in the program.  Non-members (i.e.
Visitors) enter their information via another dialog box.

At the end of the meeting checking out is accomplished with the same mechanism as when the person entered.

Later, the Chief Radio Officer may run one or more report functions to produce either a display similar
in appearance to a paper ICS-211 (ICS: Incident Command System), print the report and/or create a file
in a format suitable to Excel or other application that reads CSV files.

Some persons may forget to sign out and there is a provision for the CRO (or other officer) to add a
check out time for those persons.  Finally, there is a feature to edit both the check-in and check-out
times for a person.

The report also computes the number of hours spent by the persons attending the meeting.

If sufficient power is available during an incident the application may be used during an incident.

## Getting Started

Requires a barcode reader and a Windows 7+ PC.  The program is compiled with Visual Studio 2017 and
requres the Wix Installer add-on.

It was devloped with a Symbol BarCode Reader Model DS9208.

### Installing

The msi file is the installation program.  I believe that connecting the barcode reader to the PC is
all that required to load the drivers, or go to the Symbol Technologies website and download them.

## Running the tests

The program loads a comma separated file containing the members of the organization identifying
information when it starts up.  Otherwise the Tool Menu has a command to copy the appropriate file to
the Application's Roaming Directory.  The CSV file is produced by the RacesDB program and it copies the
information from the RACES database (an Access DB).

Once the program is running it is ready to read barcodes.  They are structured as FCC CallSign, Space,
Badge Number, Period.  The Period is needed to force the completion of the read in the software.

The member's information is found in the member info file and the date/time is taken from the PC's clock.
This information is displayed at the top of the list of check-ins and written to the end of a file
(chosen by the user when starting up).

If the file chosen for backup is an existing file, then the contents are displayed as if they had been
read from the barcode reader.  Otherwise, a incident information are requested from the user which is then
stored in the file.

A member can use a mouse and keyboard to enter his information if he/she does not have a badge with them
and a visitor may supply his information using another command than the member uses.


## Updates

### Update 3/24/23

Update the application to match the changes to reporting in the Library.

### Update 11/5/22

Update the open file dialog.  Update the help file.  Update the help file images.

### Update 10/28/22

Start app with the same position and size as when it terminated the last time.  Update the background of
the toolbar.

### Update 01/31/22

Updates to the library, mostly about the toolbar, but also NewAllocator, and a minor addition to
Expandable.

Also scrubbed the project files to remove all references to precompiled header files which I turned off
in VS17.  Unfortnately VS does not remove all references and pch files kept cropping up.  Not only does
that slow down the compiler but the files are large...

### Update 11/13/21

The changes are in the library:

  - Expandable and ExpandableP -- moved the functions out of the class to make the compilation smaller,
also fixed one bug (or feature) in the ExpandableP insert sorted function that would throw out entries
that were already in in the array.
  - FileIO -- Changed the type of path from "String&" to "TCchar*".  It turns out that automatic
conversion from String to TCchar* is easy, but the reverse is is not easy.  Also added a function to
get the length of a file given a path.
  - FineName -- Added a function to get the directory name of from a path
  - NewAllocator -- Corrected the debug dependency from if to ifdef
  - ToolBar -- Added data for drop down menus

### Update 10/31/21

Changed Resources (i.e. xxx.rc data) access so that it will work with a dll.  Changed the logic in the
About Box code.  Some other library changes.  Some corresponding changes in the application.

### Update 10/7/21

Library improvements (changes anyway!).

### Update 4/17/21

As a result of writing an article on Expandable there are subtle changes in the library.  This update
just contains changes related to the library changes...

Added some logic to allow both the Options dialog and Printer Setup dialog to set and view the current
orientation (i.e. Portrait/Landscape).

### Update 3/8/21

Rename PathDsc and change the initialization method.

### Update 2/15/21

Fixed Backup date preparation to use 2 digit year instead of century.  Worked last year due to
coincidence (ugh!!!).

### Update 2/6/21

Modified the toolbar so that the background is the same as the window's boundaries.  Updated the
icons.  Added a command to open a ICS-211a file (it is also performed at startup).

### Update 1/4/21

Most of the changes were to the Library which impacted Adjust Project File.

Library Changes:
  * CApp includes a function to identify the application to Windows.
  * CApp requires the application's "this" pointer at initialization.
  * CApp includes inline functions for retrieving a pointer to the document object and view object
  * The library now contains logic to manage the actual size of fonts on the display and printed page.
  * Before the font point size was expressed as an integer 10 x font size (e.g. 120).  Now it is
    expressed as as a decimal number of the actual point size (eg. 12.0).
  * ManipT used during output was changed to remove a memory leak.
  * The Options feature was moved to the Extras and Library.  The options include an option to adjust
    the scale of the displayed and printed output.

### Update 11/28/20

Added a separate command for creating a Excel CSV file.  The Write to File (Toolbar Floppy Icon) command
writes the current screen content to a text file.  One little glitch in this process is that the font
width on the screen is for an Ariel Font while a text file is similar to Courier.  Thus the tabs are
scaled by ~1.3 so that the columns line up correctly.

The Library has been updated also and changes were made to take that into account.

Updated the help file too.

### Update 11/07/20

Consolidating the changes uploaded previously.  Created a virtual module for the open notepad display and
printing.  The open notepad has the same problems as other reports with respect to interference between
the display and printing.  So the NoteRpt module allows separation of printing and displaying.

NoteRpt is stored in the library and provides a minimal package for dealing with open notepad output.
However, most of it is virtual and can be overridden to affect your version of the open notepad output.

### Update 11/05/20

During the Library Improvement Project I started checking each command.  This resulted in discovering
that printing was flawed.  It turned out that the simpleminded approach that I took was seriously flawed.
Windows MFC divides up the execution of a program into multiple threads, in particular the printing of
anything in the application.  So the simpleminded approach of one data structure for containing the
displayable output just would not work.

One thing led to another and the entire display/print/print preview were completely reorganized.  The
view object was put in charge of creating the display/print/print preview output with as much of the logic
being placed in the base class CScrView (in the Library) and several other classes: NoteMgr, DsplyMgr,
printMgr.  The trick to avoiding problems with multiple threads, in this case, was to use separate objects
to contain the output data for the display and print output.

I also added output, display and printed, for the Roster (list of people as they arrived), the 211
formatted log (shows check-in and check-out times), the current list of members.

It also occurred to me sometime in the last month that members could check-in and out multiple times
during an event.  This required significant changes in the organization of the Roster.

### Update 9/10/20
Library improvement project.  Changed the Expandable Array classes, added templates for a pointer only
class in the ExpandableP class definition.  It turned out I was defining this RcdPtr class many times
worrying about exactly how it should be defined.  So I turned it into a template, which of course then
I needed to go back and add it to all the programs.  Severl iterations later I added some documentation
and I am uploading the programs today.

## Built With

Slickedit is used for all edits, Visual Studio 2017 with various additions (Wix being one) is used to
build and debug.  The old fashion Help Workshop is used to compile the help file and an old copy of
Dreamweaver is used to construct the html files used in the Help Workshop.

This work was built with Unicode as the character implementation.

My current operating system is Windows 7 and I have little interest in moving to Windows 10 until it
appears to be stable.  In April 2020 it does not appear stable.

## Authors

Robert R. Van Tuyl, K6RWY, romeo romeo victor tango alpha tango sam whisky delta echo dot charlie oscar mike

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

This application is the result of an interesting conversation with Nigel, AF6ZF.  He expressed an
interest in some way of avoiding a lot of manual input of ICS 211 (Roster Information) information for
each RACES event.

