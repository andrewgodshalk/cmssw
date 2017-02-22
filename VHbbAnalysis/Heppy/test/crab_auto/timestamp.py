#/usr/bin/env python

#-------------------------------------
# timestamp.py
#
# created : 2016-12-09 - godshalk
# modified: 2016-12-09 - godshalk
#
# Function that creates a timestamp string for use in filenames.
#

import datetime

def timestamp() : 
    now = datetime.datetime.now()
    return now.strftime("%Y-%m-%d_%H%M%S") 

if __name__ == "__main__" :
    print timestamp()
