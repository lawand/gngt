GNGT 1.0.3
----------

German Noun Gender Trainer (GNGT) is a small application that helps German
learners memorize genders of German nouns by memorizing their corresponding
definite articles (der, die or das).

Copyright 2010, 2011 Omar Lawand Dalatieh <lawand87@gmail.com>.

http://coinonedge.com/gngt/


Changelog
---------

v1.0.3 (in development)
No new features yet.

v1.0.2 (2011-05-27)
A bug-fix release, no new features.

v1.0 (2010-12-12)
Added ability to store plural forms of nouns, added support for Maemo Linux.

v0.9 (2010-12-05)
Added support for Ubuntu Linux.

v0.8 (2010-11-28)
Added support for the Symbian platform.

v0.7 (2010-11-19)
Added ability to keep track of the memorization of nouns.

v0.6 (2010-11-17)
A bug-fix release, no new features.

v0.5 (2010-11-15)
Added ability to edit nouns using the program interface.

v0.4 (2010-11-12)
Added ability to fix erroneous lines at startup, writing nouns to nouns file at
exit.

v0.3 (2010-11-10)
A bug-fix release, no new features.


Installation
------------

Windows:

GNGT is distributed as a portable executable file, you don't have to install
it. You can start it from any folder.

Supported versions of the Windows operating system are Windows XP, Windows
Vista and Windows 7.

Ubuntu Linux:

Extract and double click the .deb file. Follow the on-screen instructions.

Ubuntu binary was tested with Ubuntu 10.10.

Maemo Linux:

Extract and send the .deb file to your device. Open it and follow the
on-screen instructions.

Maemo binary was tested with Maemo5 PR1.3

Symbian:

Devices compatible with GNGT are those compatible with Qt. In other words,
most Nokia smart phones released after 2006 and some smart phones of other
brands. More specifically, Symbian powered smart phones that run S60 versions
3.1, 3.2 or 5.0. Plus models that run Symbian^3 and (probably) later versions
of Symbian.

If you don't know if your device fits the previous description, simply download
and install GNGT and Qt, and if you got no error messages during installation
then your device is probably compatible with GNGT.

Note for Nokia E-Series users: you need to change certain settings to be able
to install the program, in case an error message appeared during installation.
This setting can be changed in the device as follows: Open the application
manager, then select: Settings > Software installation > All.


Usage
-----

Before you can start using the program, you should add some nouns and set their
genders. You can do so by selecting the "edit nouns" option from the file menu.
You can modify the nouns later in the same way.

It's not recommended to add compound nouns because the program interface won't
fit long nouns, and memorizing the gender of the last part of a compound noun
is sufficient. The program allows a maximum of 23 characters for both the
singular form and the plural form of any noun.

When adding new nouns, a singular form must be entered, but a plural form is
optional. However, it's highly recommended to look up plural forms and add them
so that both singular and plural forms are memorized.

After adding some nouns, you can start using the program which would test you
by giving you some random noun (i.e. Buch) and you should supply the correct
corresponding definite article (which is in this case 'das'). If you made a
mistake, you'll get a corrective feedback. And either ways, you'll get another
random noun afterwards, and so on.

Each noun has a memorization streak, that is, the number of sequential times
that it's gender was chosen correctly by the user. A single mistake resets the
streak back to 0. When a streak reaches 10, the corresponding noun is
considered memorized, and is removed.

All nouns are stored in a text file named "nouns.txt". You can copy the file
into another platform and use it there because the format and character
encoding of the file is consistent across all platforms.

Windows Platform Usage Notes:

The nouns file "noun.txt" should be placed in the same directory in which
GNGT is.

Keyboard shortcuts are indicated by underlining the letters that correspond
with the button which should be pressed together with the Alt key to access
the function. For instance, to choose the masculine definite article button
(der) in the main window you should press the Alt button together with the R
button because the letter 'r' is underlined.

Underlining might not appear automatically, in which case you should press the
Alt key to make it appear.

Only one shortcut isn't indicated, and that is the delete key which removes
highlighted nouns in the edit nouns dialog.

Ubuntu Linux Platform Usage Notes:

The nouns file "nouns.txt" is stored at "~/.gngt/nouns.txt", which would
typically be /home/USERNAME/.gngt/nouns.txt. Note that the directory "~/.gngt/"
is typically hidden.

The nouns file doesn't get deleted when uninstalling GNGT.

Keyboard shortcuts are indicated by underlining the letters that correspond
with the button which should be pressed together with the Alt key to access
the function. For instance, to choose the masculine definite article button
(der) in the main window you should press the Alt button together with the R
button because the letter 'r' is underlined.

Underlining might not appear automatically, in which case you should press the
Alt key to make it appear.

Only one shortcut isn't indicated, and that is the delete key which removes
highlighted nouns in the edit nouns dialog.

Maemo Linux Platform Usage Notes:

The nouns file "nouns.txt" is stored at "/home/user/MyDocs/Others/nouns.txt",
and doesn't get deleted when uninstalling GNGT.

Tip: To enter a lot of nouns quickly, open the edit nouns dialog, and don't use
the touch screen to click on any button, instead use the keyboard. Press the
Enter button to add a new noun and press it again after typing the noun to
confirm. You can then repeat this process as much as needed.

Symbian Platform Usage Notes:

The nouns file "nouns.txt" is stored at "C:\data\Others\", and doesn't get
deleted when uninstalling GNGT.

On smart phones that don't have a touch screen, there are some numpad shortcuts
which are indicated by mentioning the number that corresponds with the button
that should be pressed to access the function.

Note: If some text in the program interface wasn't readable, try changing the
device theme (default themes are usually well-designed regarding this aspect)


Advanced Usage
--------------

The character encoding of the nouns file 'nouns.txt' is UTF-8. You can edit the
file manually but it's recommended for you to edit the nouns using the program
interface because the default character encoding on some operating systems
isn't UTF-8, which means that you have to specify this character encoding when
saving the file instead of just saving it using default options.

The nouns in the nouns file should be each in a different line composed of the
singular form of the noun preceded by it's corresponding definite article and
(optionally) followed by the plural form and then (again optionally) followed
by the memorization streak (i.e. 'das Auto Autos' (without the quotes)). Note
that erroneous lines will be reported by the program at startup, and you can
fix them using the program interface.

Duplicates get removed when reading nouns at startup.

The noun list gets saved to the nouns file every time the program is closed,
and also every time the "Edit Nouns" dialog is closed.


Development
-----------

GNGT was written in Qt 4.

GNGT version numbers follow this scheme: Major.Minor.Patch (e.g 0.8.2).
Released versions (which are git-tagged) always have an even patch number
(e.g. 1.7.2). Odd patch numbers are given to code edits that take place between
two releases (e.g. 1.7.3 is between 1.7.2 the next released version which could
be 1.7.4 and could be 1.8.0 and 2.0.0 depending on the severity of the changes
made to the code).
This is true starting with 1.0.2.

GNGT was tested to be built with Qt 4.7.0 for Windows, Ubuntu Linux and Maemo
Linux, and tested to be built with Qt 4.6.3 for Symbian.
It's also tested to be ran with Qt 4.7.0 for Windows, Ubuntu Linux and Maemo
Linux, and tested to be ran with Qt 4.7.1 for Symbian.

Packaging GNGT for Ubuntu Linux:

There are many methods of creating .deb package files for Ubuntu Linux. This is
the method I am using:
    Get a machine running Linux.
    Build GNGT for Linux, the result is a binary file named "gngt".
    Put the binary in qtc_packaging/ubuntu/gngt_1.0.3_i386/usr/bin.
    Open the terminal and change the current directory to qtc_packaging/ubuntu.
    Execute this command: "fakeroot dpkg-deb -b gngt_1.0.3_i386/".
    The resulting file is "gngt_1.0.3_i386.deb" which can be used to install GNGT.

The distribution tested is Ubuntu 11.04.

Packaging GNGT for Maemo Linux:

Simply open gngt.pro with Qt Creator and select the Maemo target and build and
create a package for GNGT.

This was tested with Qt SDK 1.1.1 and Maemo5 PR1.3.

Symbian Note:

Distributing a custom build requires renaming the application (so that it can
be installed alongside with GNGT), and (as it is with all Symbian applications)
specifying a UID.

GNGT for Symbian was tested Qt SDK 1.1.1 and with Qt 4.6.3 for Symbian^1 and
using the Nokia smart installer on S60v3 FP1, S60v3 FP2, Symbian^1, Symbian^3.


Legal Notes
-----------

Copyright 2010, 2011 Omar Lawand Dalatieh.

GNGT is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

GNGT is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
GNGT. If not, see <http://www.gnu.org/licenses/>.

All trademarks are property of their respective owners.


Credits
-------

GNGT was programmed using Qt, Qt SDK and Nokia Qt SDK.

GNGT uses UPX (http://upx.sourceforge.net/) for executable compression.

Debreate (http://debreate.sourceforge.net/) is used for Ubuntu packaging.

Program icon was designed using the Inkscape SVG graphics editor.

