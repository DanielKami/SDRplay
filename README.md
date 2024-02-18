This is another version of LibMiriSDR-4, but it is corrected for Windows and more stable. The great honor for the hard work should be given to Miroslav Slugen and the addition of Leif Asbrink SM5BSZ. If you are using the code to quote these two authors, they spent many hours breaking down the meaning of registers in the not-documented chip.


* Support for Windows 10/11 through WinUSB (the newest version)The bulk synchronous transfer is improved.
* The code is as simple as possible with all the original comments.
* I solved the problem with transfer stability during the change of rate.
* The code supports all the futures of the original one, but now for Windows.
* The problem with Windows USB transfers is also solved.
* I didn't test the synchronous mode of USB, but the rest works well.
* The library dynamically links the libusb.dll (in the newest version).
* 

Cooperation is really apreciated:)


With this code it isa easynow to create interfaces for other software.
