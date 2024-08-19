----------------------UNIX-FileSystem-----------------------
COMMAND   USAGE                                   INSTRUCTION
help      help                                    Display overview of functionalities
fformat   fformat                                 Format file volume
mkdir     mkdir <dirname>                         Create directory
ls        ls                                      List directory
scp       scp [-u/-d] <from> <to>                 Upload / Download file from file system
fread     fread <fd> <nbytes>                     Read file
fcreate   fcreate <filename>                      Create file
fopen     fopen <filename>                        Open file
fclose    fclose <fd>                             Close file
fwrite    fwrite <fd> <nbytes> <string>           Write file
flseek    flseek <fd> <offset> <seekmode>         Move file pointer, seekmode: {0:set, 1:addcur, 2:addend, 3-5:512unit}
fdelete   fdelete <filename>                      Delete file
cd        cd <dirname>                            Change current directory
cp        cp <file1> <file2>                      Copy file
frename   frename <file1> <file2>                 Rename file
ftree     ftree <dirname>                         Display directory tree
pwd       pwd                                     Display current directory
login     login <username>                        Login

logout    logout <username>                       Logout
exit      exit                                    Exit
----------------------UNIX-FileSystem-----------------------
