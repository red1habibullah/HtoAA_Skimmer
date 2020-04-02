Ntuplizer
=========

The ntuplizer produces flat root files from MiniAOD EDM files.

Events are stored if they include at least one electron, muon, or tau.

Usage
-----

Use the configuration file [MiniTree_cfg.py](test/MiniTree_cfg.py).
This configuration supports the following files:
 * MC
   * RunIIFall17MiniAOD
 * Data
   * EOY ReReco 2017

Options:
 * `inputFiles`: Standard cmsRun inputFiles argument for PoolSource.
 * `outputFile`: Standard cmsRun outputFile argument (uses TFileService). Default: `miniTree.root`.
 * `isMC`: Use if you are running over Monte Carlo. Default: `0` (for data).

### Example

 * Data
```
validateNtuples.py data
```

 * MC
```
validateNtuples.py mc
```

Grid Submission
---------------

Jobs can be submitted to the grid using the submit_job.py script (in DevTools/Utilities package).

See `submit_job.py -h` for help.

You must first source the crab environment:

```
source /cvmfs/cms.cern.ch/crab3/crab.sh
```

The `--dryrun` option will tell crab to submit a test job and report the success or failure.
It will also give you estimated runtimes. When you are ready to submit, remove the `--dryrun` option.
You will also need to change the `jobName` option.

### Example

 * Data
```
submit_job.py crabSubmit --sampleList datasetList_Data.txt --applyLumiMask --dryrun testDataSubmission_v1 DevTools/Ntuplizer/test/MiniTree_cfg.py
```

 * MC
```
submit_job.py crabSubmit --sampleList datasetList_MC.txt --dryrun testMCSubmission_v1 DevTools/Ntuplizer/test/MiniTree_cfg.py isMC=1
```

