# Pointtools

These  tools  are scripts and ways to present point files. They are pre‐
formatted text files presented in catpoint(1). In /bin is  a  script  to
easily convert markdown files to point files.

See the examples/hello directory for how to use it in real life:

	% git clone git://git.2f30.org/catpoint
	% cd catpoint
	% make
	% cp catpoint $HOME/bin
	% cp ../bin/* $HOME/bin
	% cd ../examples/hello
	% cat hello.md | md2point
	% catpoint *.txt

There are not many formatting options for now, but it’s enough for daily
usage. If you need any graph or formatting, use  troff(1)  and  all  the
formatting available there.

md2point requires utf8expr gopher:// bitreich.org/1/scm/utf8expr/log.gph

Have fun!

