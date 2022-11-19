# infer from set to set

these 14 functions return inference properties of the set of objects obj is managing at this time (a subset of the objects you've obj_class_observe()d)

    double obj_class_indifreq(obj_t indicator, obj_t target, long type);

the following 13 functions use the same parameters as above

    double obj_class_targfreq(..);
    double obj_class_indiover(..);
    double obj_class_targover(..);
    double obj_class_over(..);
    double obj_class_indimis(..);
    double obj_class_targmis(..);
    double obj_class_indiimp(..);
    double obj_class_targimp(..);
    double obj_class_indiopac(..);
    double obj_class_targopac(..);
    double obj_class_inditrans(..);
    double obj_class_targtrans(..);
    double obj_class_trans(..);

some of the names begin with *indi* or *targ*--those mean indicator or target--*freq* means frequency--*over* means overlap--*mis* means mismatch--*imp* means impertinence--*opac* means opacity--and *trans* means transparency--so indifreq() returns the indicator frequency--targfreq() returns the target frequency--etc

each takes an indicator and a target (as well as the usual type parameter where you specify MUSHROOM--GAME_MAP--etc) the indicator and target are obj_t types--but they are interpreted as a type mask which represents a set of objects--a 0 bit in these variables means that attribute field is not used in matching objects to the type--a 1 bit in these variables means an object must also have a 1 in that field in order to match the type

## indicator frequency

    indicator / target

this shows the rate at which the indicator set grows with respect to the growth of the target set--that is--the size or cardinality of the set of objects defined by *indicator* divided by the cardinality of the set defined by *target*

if *indicator* and *target* grow at the same rate then indicator frequency will be 1--if *indicator* grows (or occurs) at twice the rate of *target* then indicator frequency will be 2--if *indicator* occurs at half the rate of *target* then indicator frequency will be 1/2--so this is the frequency with which the indicator occurs with respect to the target--when it is high there are a lot of indicator objects for every target object--when it is low there are a lot of target objects for every indicator object--this provides one aspect of the inference fingerprint for this set of objects

## target frequency

    target / indicator

this is the target set as a part of the indicator set--the number of objects defined as members of the *target* set divided by the number of objects defined by *indicator*

when this is below 1 there are many indicator objects per target object

## indicator overlap

    intersection(indicator, target) / indicator

indicator overlap is the same as conditional probability--indicator overlap is the degree to which *indicator* implies *target*

## target overlap

    intersection(indicator, target) / target

target overlap is the degree to which the target set is being indicated by (or targeted by) the indicator set--you can use this to construct sets that resist counterinference (by modifying your indicator set to minimize the value of this calculation)

## overlap

    intersection(indicator, target) / union(indicator, target)

when overlap is close to 0 the overlap between indicator and target is small--when overlap is close to 1 the overlap between indicator and target set is large--when close to 0 this means not much inference is possible but the quality of inferences will be high--when close to 1 this means lots of inference is possible but the quality of inferences will be low

## indicator mismatch

    (indicator - target) / (target - indicator)

is the degree to which the indicator set is unsuitable for inferring the target set

## target mismatch

    (target - indicator) / (indicator - target)

is the degree to which the target set is unsuitable for targeting the indicator set

## indicator impertinence

    (indicator - target) / target

the degree to which the indicator set is irrelevant or impertinent to the target set--a high number here means that almost all the indicator set is useless inferring the target set

## target impertinence

    (target - indicator) / indicator

the degree to which the target set is impertinent to the indicator set--a high number here means that almost all the target set is useless targeting the indicator set

## indicator opacity

    (indicator - target) / intersection(indicator, target)

the greater the indicator opacity the more opaque the indicator set--this means it doesnt reveal capacity for inference--the smaller the indicator opacity the greater the capacity for inference

## target opacity

    (target - indicator) / intersection(indicator, target)

the greater the target opacity the more opaque the target set--this means it doesnt reveal capacity for targeting--the smaller the target opacity the greater the capacity for targeting

## indicator transparency

    intersection(indicator, target) / (indicator - target)

this can be thought of as an extent to which (or a thoroughness with which) the target set invades the indicator set--it can be thought of as a degree to which the target set disects the indicator set

## target transparency

    intersection(indicator, target) / (target - indicator)

this can be thought of as an extent to which (or a thoroughness with which) the indicator set invades the target set--it can be thought of as a degree to which the indicator set disects the target set

## transparency

    intersection(indicator, target) / xor(indicator, target)

transparency is the ratio between the overlap and the xor of the indicator and target sets---as this approaches 0 the indicator and target sets are less and less related in terms of inference--as this approaches infinity the indicator set and the target set are more and more related in terms of inference--as it approaches infinity (though) the indicator set and the target set are identical and inference between them is meaningless
