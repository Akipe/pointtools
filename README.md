# Pointtools

These  tools  are scripts and ways to present point files. They are pre‐
formatted text files presented in catpoint(1). In /bin is  a  script  to
easily convert markdown files to point files.

To install md2point and dir2point, do:

	% make
	% sudo make install

To get catpoint:

	% git clone git://r-36.net/catpoint
	% cd catpoint
	% make
	% cp catpoint $HOME/bin

To view the examples:

	% cd ../examples/hello
	% cat hello.md | md2point
	% catpoint *.txt

	OR

	% dir2point .


There are not many formatting options for now, but it’s enough for daily
usage. If you need any graph or formatting, use  troff(1)  and  all  the
formatting available there.

Have fun!

