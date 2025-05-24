# This document would be the exemple structure that this project would create

Source: [Reddit comment on C project structure](https://www.reddit.com/r/C_Programming/comments/tt0q0n/comment/i2vi4lo/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button)

Consider that a folder with a few files don't give you any benefits like for example in Java.

The approach I see quite often for medium sized projects is a include folder, that includes every header file. So the project may look like this:

├── Makefile

├── //other config files, for example README.md, .gitignore

├── src/

│   └── *.c

├── include/

│   └── *.h

└── tests/

| └── some-unit-tests.c

If you tend to include third party libraries, i would probably extend this structure to this:

├── Makefile

├── //other config files, for example README.md, .gitignore

├── src/

│   └── *.c

├── include/

│   └── *.h

├── lib/

│   └── some-third-party-libraries

└── tests/

| └── some-unit-tests.c

In my opinion that is a clean, and readable approach.

If your project is really large, consider grouping related files together. For example look at the linux source code. This amount of files would overwhelm everybody, if they weren't grouped together (note, that they also use the /include approach).

[https://github.com/torvalds/linux/tree/master/include](https://github.com/torvalds/linux/tree/master/include)
