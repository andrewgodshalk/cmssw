#!/bin/bash

# Setup detailed at http://cms-service-lumi.web.cern.ch/cms-service-lumi/brilwsdoc.html#prerequisite

setenv PATH $HOME/.local/bin:/afs/cern.ch/cms/lumi/brilconda-1.1.7/bin:$PATH
#setenv PATH $HOME/.local/bin:/nfshome0/lumipro/brilconda/bin:$PATH
#setenv PATH <brilcondabasedir> /bin:$PATH

mkdir mergedJSON brilcalcResults

pip install --install-option="--prefix=$HOME/.local" brilws
pip show brilwas
