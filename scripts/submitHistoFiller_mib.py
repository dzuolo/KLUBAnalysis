#!/usr/bin/env python


###### LAUNCH COMMAND EXAMPLE:
#
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_MuTau.cfg  --tag analysis_MuTau_14Jul2018  --n 44
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_MuTau.cfg  --tag analysis_MuTau_14Jul2018_VBF --n 44
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_ETau.cfg   --tag analysis_ETau_14Jul2018   --n 44
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_TauTau.cfg --tag analysis_TauTau_14Jul2018 --n 44
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_TauTau.cfg --tag analysis_TauTau_14Jul2018_SStight --n 44
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_TauTau.cfg --tag analysis_TauTau_14Jul2018_style --n 44
#
# python scripts/submitHistoFiller_mib.py --cfg config/lookAtQCD_VBF_MuTau.cfg  --tag lookatQCD_MuTau_14Jul2018  --n 20
# python scripts/submitHistoFiller_mib.py --cfg config/lookAtQCD_VBF_ETau.cfg   --tag lookatQCD_ETau_14Jul2018   --n 40
# python scripts/submitHistoFiller_mib.py --cfg config/lookAtQCD_VBF_TauTau.cfg --tag lookatQCD_TauTau_14Jul2018 --n 44
# python scripts/submitHistoFiller_mib.py --cfg config/lookAtQCD_VBF_TauTau.cfg --tag lookatQCD_TauTau_14Jul2018_oldIso --n 44
#
################

import os
import sys
import argparse
import datetime

parser = argparse.ArgumentParser(description='Command line parser of plotting options')
parser.add_argument('--cfg',    dest='cfg',   help='name of the main cfg to run', default=None)
parser.add_argument('--tag',    dest='tag',   help='name of working space (defaults to timestamp)', default=None)
parser.add_argument('--njobs',  dest='njobs', type=int, help='number of jobs for parallelization',  default=17)
args = parser.parse_args()


if not args.cfg:
    print "** Please provide a cfg name: --cfg <relative path from KLUBanalysis/ or absolute path>"
    sys.exit()

# datetime.datetime.now()
outDir = datetime.datetime.now().strftime('%Y.%m.%d_%H.%M.%S')
if args.tag:
    outDir = args.tag
#outDir = "JEC_jobs_pt25/"+outDir

# pathname = os.path.dirname(sys.argv[0])        
# here     = os.path.abspath(pathname)
here = os.getcwd()

program = 'testAnalysisHelper.exe'

proto = 'filler_' ## job .sh fie name
logproto = 'log_' ## job .sh fie name

os.system('mkdir ' + outDir)
for nj in range(0, args.njobs):
    scriptName = proto + str(nj) + '.sh'
    logName    = logproto + str(nj) + '.txt'
    scriptFile = open (outDir + '/' + scriptName, 'w')
    scriptFile.write ('#!/bin/bash\n')
    scriptFile.write ('source /cvmfs/cms.cern.ch/cmsset_default.sh\n')
    scriptFile.write ('eval `scram r -sh`\n')
    scriptFile.write ('cd %s\n' % here)
    scriptFile.write ('eval `scram r -sh`\n')
    scriptFile.write ('source scripts/setup.sh\n')
    command = program + ' ' + args.cfg + ' ' + str(nj) + ' ' + str(args.njobs) + ' 2>&1 | tee ' + outDir + '/' + logName
    scriptFile.write(command)
    scriptFile.close()

    
    os.system ('chmod u+rwx ' + outDir + '/' + scriptName)
    launchcommand = ('/usr/bin/qsub -q  longcms ' + outDir + '/' + scriptName)
    print launchcommand
    os.system (launchcommand)
