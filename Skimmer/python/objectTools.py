import FWCore.ParameterSet.Config as cms

def getCapitalizedSingular(name):
    if name=='mets': return 'MET'
    return name.rstrip('s').capitalize()

def objectSelector(process,obj,objSrc,selection,postfix='',patType=None):
    '''Filter an object collection based on a cut string'''

    if not patType: patType = getCapitalizedSingular(obj)

    if obj=='genParticles':
        module = cms.EDFilter("GenParticleSelector",
            src = cms.InputTag(objSrc),
            cut = cms.string(selection),
            filter = cms.bool(False)
        )
    else:
        module = cms.EDFilter(
            "PAT{0}Selector".format(patType),
            src = cms.InputTag(objSrc),
            cut = cms.string(selection),
        )
    modName = '{0}Selected{1}'.format(obj,postfix)
    setattr(process,modName,module)

    pathName = '{0}SelectionPath{1}'.format(obj,postfix)
    path = cms.Path(getattr(process,modName))
    setattr(process,pathName,path)

    process.schedule.append(getattr(process,pathName))

    return modName

def objectCountFilter(process,path,obj,objSrc,count,postfix=''):
    module = cms.EDFilter("PATCandViewCountFilter",
        minNumber = cms.uint32(count),
        maxNumber = cms.uint32(999999),
        src = cms.InputTag(objSrc)
    )
    modName = '{0}Count{1}'.format(obj,postfix)
    setattr(process,modName,module)
    path *= getattr(process,modName)


def objectCleaner(process,obj,objSrc,collections,cleaning,postfix='',patType=None):
    '''Clean an object collection'''

    if not patType: patType = getCapitalizedSingular(obj)

    cleanParams = cms.PSet()
    for cleanObj in cleaning:
        cleanSrc = collections[cleanObj]
        cut = cleaning[cleanObj]['cut']
        dr  = cleaning[cleanObj]['dr']

        particleParams = cms.PSet(
            src=cms.InputTag(cleanSrc),
            algorithm=cms.string("byDeltaR"),
            preselection=cms.string(cut),
            deltaR=cms.double(dr),
            checkRecoComponents=cms.bool(False),
            pairCut=cms.string(''),
            requireNoOverlaps=cms.bool(True),
        )

        setattr(cleanParams,cleanObj,particleParams)

    module = cms.EDProducer(
        "PAT{0}Cleaner".format(patType),
        src = cms.InputTag(objSrc),
        preselection = cms.string(''),
        checkOverlaps = cleanParams,
        finalCut = cms.string(''),
    )
    modName = '{0}Cleaned{1}'.format(obj,postfix)
    setattr(process,modName,module)

    pathName = '{0}CleaningPath{1}'.format(obj,postfix)
    path = cms.Path(getattr(process,modName))
    setattr(process,pathName,path)

    process.schedule.append(getattr(process,pathName))

    return modName
