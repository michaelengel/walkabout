#
# Copyright (C) 2001, Sun Microsystems, Inc
#
# See the file "LICENSE.TERMS" for information on usage and
# redistribution of this file, and for a DISCLAIMER OF ALL
# WARRANTIES.
#
#

#######################################################################
# FILE:     mt-tests.tcl
# OVERVIEW: Tcl script that runs in parallel the regression tests for
#           interpreters generated from the Walkabout framework.
#######################################################################

# To run: 
#       tclsh mt-tests.tcl <source> [other-tcl-options>]
#       where <source> is one of 'sparc' or 'pentium'  
#
#  8 Oct 01 - Brian: created file.
# 24 Oct 01 - Brian: Corrected the command that generates the output
#              directory name.

###
### Usage: display usage information for this test script and exit
###

proc usage {} {
    puts "Usage: tclsh mt-tests.tcl <source> <other-tcltest-options>"
    puts "       where <source> is either \"sparc\" or \"pentium\"."
    puts "       For other tcltest arguments, see the tcltest man page:"
    puts "       e.g., -debug 1 turns on debugging."
    puts ""
    puts "Tests that use the SPEC reference input files take a very long time"
    puts "and are disabled by default. Add \"-constraints refInput\" to the"
    puts "command line to enable these."
    exit
}

###
### The tests must be run in the dynamic/test subdirectory.
###

set dirList [file split [pwd]]
set lastDir [lindex $dirList end]
if {[string compare $lastDir "test"] != 0} {
    puts "Error: Walkabout tests can only be run in the directory <uqbt>/dynamic/test"
    exit
}

###
### All arguments are passed on to the individual tests. There should be an
### even number of arguments.
###

if {$argc < 1} {
    usage
}
set testArgv $argv

###
### The set of tests to run: the cross-product of the following three sets.
###
### +-----------------------------------------------------------------------+
### | NB: KEEP THESE LISTS IN SYNC WITH THOSE IN interp-regression.tcl!     |
### +-----------------------------------------------------------------------+
###

set benchmarks  {compress perl go ijpeg}
set extensions  {v8}
set tuningTypes {peak base}
set inputSets   {test}

###
### Determine the name of the directory in which to deposit the output files
### for each test.
###

set dirName "[exec /proj/walkabout/bin/date {+walktest-%b-%d-%Y-%H:%M:%S}]"
file mkdir $dirName

puts "Parallel Walkabout tests"
puts "Writing results into directory $dirName"

###
### Also do the tests that use the SPEC reference input files if
### "-constraints refInput" is specified on the command line.
###

set cnstrIdx [lsearch $argv "-constraints"]
if {$cnstrIdx >= 0} {
    if {[lsearch $argv "refInput"]} {
        puts "Also doing tests that use the SPEC reference input files"
        lappend inputSets ref
    }
}

###
### Now run each test in parallel, with output into separate files in $dirName.
###

foreach bm $benchmarks {
    foreach ext $extensions {
        foreach tune $tuningTypes {
            foreach input $inputSets {
                set testName $bm.$ext.$tune.$input
                
                set fileName "$dirName/$testName.out"
                set cmd "/proj/walkabout/bin/tcltest interp-regression.tcl $testArgv -match $testName"
                #puts "Forking command \"$cmd\" with output file $fileName"
                set pid [eval exec $cmd >& $fileName &]
                puts "Forked process $pid to execute test $testName"
            }
        }
    }
}
