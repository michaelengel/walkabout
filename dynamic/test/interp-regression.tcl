#
# Copyright (C) 2001, Sun Microsystems, Inc
#
# See the file "LICENSE.TERMS" for information on usage and
# redistribution of this file, and for a DISCLAIMER OF ALL
# WARRANTIES.
#
#

#######################################################################
# FILE:     interp-regression.tcl
# OVERVIEW: Tcl script file to test interpreters generated from the 
#           Walkabout framework
#######################################################################

# To run: 
#       tclsh interp-regression.tcl <source>
#       where <source> is one of 'sparc' or 'pentium'  
# Note: 
# You may need to change the paths in the PATHS section below
#
# 21 Sep 01 - Cristina: created based on uqbt's regression.test tcl script
# 25 Sep 01 - Brian: Small edits to expected results and to ensure correct
#              arguments are passed to compress.
# 26 Sep 01 - Cristina: added perl, go and ijpeg tests
#                 4 standard tests for each benchmark: base and peak 
#                 optimizations with test and reference data sets.
#  5 Oct 01 - Brian: Enabled tests where executables had not been built 
#              formerly. Removed some dangling double-quotes.
#  8 Oct 01 - Brian: Removed unused execSed() procedure. Corrected argv
#              adjustment for odd argc to allow other tcltest command line
#              arguments to be given. Corrected several calls to execCmd().
#              Use process id to distinguish different stderr output files
#              so that tests can be run in parallel. Added script name to perl
#              tests. Changed use of "echo <file> | <cmd>" to "<cmd> < <file>".
#              Added "refInput" constraint to tests that use the SPEC reference
#              input files to disable these unless "-constraints refInput" is
#              on the cmd line since these tests take hours to run. Fixed the
#              ijpeg tests, which require an input filename, not its contents.
#              Added output of start and finish time.
# 24 Oct 01 - Brian: Updated some expected instruction counts.
#

############################################################################## 
### Note on usage
############################################################################## 
#
# Look at the tcltest man page for information about the use of the following
# command line parameters:
#    -constraints refInput (also run tests marked with the "refInput" contraint
#                           in the 4th argument of their test command)
#    -match 'go*'          (run only tests with names that match the specified
#                           patterns)
#    -skip 'go* ijpeg*'    (skip tests with names that match the specified
#                           patterns)


############################################################################## 
### Initial setup: process command line, load tcltest package, etc.
############################################################################## 

###
### usage: displays usage information for this test script and exits
###

proc usage {} {
    puts "Usage: tclsh interp-regression.tcl <source> <other-tcltest-options>"
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
### Process command-line arguments
###

###
### Extract <source> (the machine on which to run) from argv then remove that
### first argument.
###

if {$argc < 1} {
    usage
}
set SOURCE [lindex $argv 0]
set  argv [lrange $argv 1 end]
incr argc -1

###
### The remaining arguments should be even in number.
###

if {$argc % 2 != 0} {
    puts "Number of arguments must be even, is [incr argc]:"
    puts "\t\targv: \"$SOURCE $argv\""
    usage
}

###
### Load the test support: <tclhome>/library/tcltest1.0/tcltest.tcl
###

if {[lsearch [namespace children] ::tcltest] == -1} {
    package require tcltest
    namespace import -force ::tcltest::*
}

###
### Turn on/off verbose output by uncommenting the line below. You can also
### control this with the "-verbose" command line argument; see "man tcltest"
### for details.
###

#set ::tcltest::verbose p


if {$::tcltest::debug >= 1} {
    puts "Source = $SOURCE"
}

set dirList [file split [pwd]]
set lastDir [lindex $dirList end]
if {[string compare $lastDir "test"] != 0} {
    puts "Error: the Walkabout tests can only be run in the directory uqbt/dynamic/test"
    exit
}
set WALKABOUTPATH [file dirname [pwd]]
set EMUPATH "$WALKABOUTPATH/emu"


############################## PATHS Section #############################
################ Start of changes to the local environment ###############
########## You can change the local paths to the programs below ##########

###
### Set up paths according to your site. 
###

if {[string equal $SOURCE "sparc"]} {
    set SPARCEMU sparcemu
    set TESTPATH $WALKABOUTPATH/test/sparc
    set OUTPATH /tmp
} elseif {[string equal $SOURCE "pentium"]} {
    set PENTEMU x86emu
    set TESTPATH $WALKABOUTPATH/test/pentium
    set OUTPATH /tmp
} else {
    usage
}
if {$::tcltest::debug >= 1} {
    puts "WALKABOUTPATH = $WALKABOUTPATH"
    puts "TESTPATH      = $TESTPATH"
    puts "OUTPATH       = $OUTPATH"
}


########################################################################### 
################# End of changes to the local environment #################
########## You shouldn't need to change anything below this line ##########
########################################################################### 


############################################################################## 
### Support routines for tests 
############################################################################## 


###
### Determine the processor on which the tests are running 
###

set PROCESSOR [exec uname -p]
puts "Walkabout tests: host processor is $PROCESSOR"
set start [clock seconds]
puts "Tests started [clock format $start]" 


###
### execCmd: executes a command while redirecting its stdout and stderr
###

proc execCmd {cmd stdoutVarName stderrVarName} {
    upvar $stdoutVarName stdoutVar
    upvar $stderrVarName stderrVar
    set stderrFilename "interp-regression-stderr-[pid].file"
    ::tcltest::makeFile "" $stderrFilename
    set result [catch {eval exec $cmd 2> $stderrFilename} stdoutVar]
    set stderrVar [::tcltest::viewFile $stderrFilename]
    ::tcltest::removeFile $stderrFilename
    return $result
}


############################################################################## 
### Tests start here
############################################################################## 


if {[string equal $SOURCE "sparc"]} {

###
### compress.v8 base optimization, test data set
###

    test compress.v8.base.test {$SPARCEMU compress.v8.base.test} {
        set myStdout ""
        set myStderr ""
        set program "/proj/walkabout/spec95/benchspec/CINT95/129.compress/exe/base/compress95.v8"
        set inData [::tcltest::viewFile /proj/walkabout/spec95/benchspec/CINT95/129.compress/data/test/input/test.in] 
        set result [execCmd "echo $inData | $EMUPATH/$SPARCEMU $program" myStdout myStderr]
        set outData [::tcltest::viewFile /proj/walkabout/spec95/benchspec/CINT95/129.compress/data/test/output/test.out] 
        list $result [string equal $myStdout $outData] $myStderr
    } {0 1 {Interpreted 2581314 instructions}}


###
### compress.v8 base optimization, reference data set
###

    test compress.v8.base.ref {$SPARCEMU compress.v8.base.ref} {refInput} {
        set myStdout ""
        set myStderr ""
        set program "/proj/walkabout/spec95/benchspec/CINT95/129.compress/exe/base/compress95.v8"
        set inData [::tcltest::viewFile /proj/walkabout/spec95/benchspec/CINT95/129.compress/data/ref/input/bigtest.in]
        set result [execCmd "echo $inData | $EMUPATH/$SPARCEMU $program" myStdout myStderr]
        set outData [::tcltest::viewFile /proj/walkabout/spec95/benchspec/CINT95/129.compress/data/ref/output/bigtest.out]
        list $result [string equal $myStdout $outData] $myStderr
    } {0 1 {Interpreted 3681016 instructions}}

###
### compress.v8 peak optimization, test data set
###

    test compress.v8.peak.test {$SPARCEMU compress.v8.peak.test} {
        set myStdout ""
        set myStderr ""
        set program "/proj/walkabout/spec95/benchspec/CINT95/129.compress/exe/peak/compress95.v8"
        set inData [::tcltest::viewFile /proj/walkabout/spec95/benchspec/CINT95/129.compress/data/test/input/test.in] 
        set result [execCmd "echo $inData | $EMUPATH/$SPARCEMU $program" myStdout myStderr]
        set outData [::tcltest::viewFile /proj/walkabout/spec95/benchspec/CINT95/129.compress/data/test/output/test.out] 
        list $result [string equal $myStdout $outData] $myStderr
    } {0 1 {Interpreted 2581314 instructions}}


###
### compress.v8 peak optimization, reference data set
###

    test compress.v8.peak.ref {$SPARCEMU compress.v8.peak.ref} {refInput} {
        set myStdout ""
        set myStderr ""
        set program "/proj/walkabout/spec95/benchspec/CINT95/129.compress/exe/pe
ak/compress95.v8"
        set inData [::tcltest::viewFile /proj/walkabout/spec95/benchspec/CINT95/129.compress/data/ref/input/bigtest.in]
        set result [execCmd "echo $inData | $EMUPATH/$SPARCEMU $program" myStdout myStderr]
        set outData [::tcltest::viewFile /proj/walkabout/spec95/benchspec/CINT95/129.compress/data/ref/output/bigtest.out]
        list $result [string equal $myStdout $outData] $myStderr
    } {0 1 {Interpreted 3681016 instructions}}


###
### perl.v8 base optimization, test data set
###

    test perl.v8.base.test {$SPARCEMU perl.v8.base.test} {
        set myStdout ""
        set myStderr ""
        set program "/proj/walkabout/spec95/benchspec/CINT95/134.perl/exe/base/perl.v8"
        set script  "/proj/walkabout/spec95/benchspec/CINT95/134.perl/data/test/input/primes.pl"
        set inFile  "/proj/walkabout/spec95/benchspec/CINT95/134.perl/data/test/input/primes.in"
        set result [execCmd "$EMUPATH/$SPARCEMU $program $script < $inFile" myStdout myStderr]
        set outData [::tcltest::viewFile /proj/walkabout/spec95/benchspec/CINT95/134.perl/data/test/output/primes.out]
        list $result [string equal $myStdout $outData] $myStderr
    } {0 1 {Interpreted 10365419 instructions}}


###
### perl.v8 base optimization, reference data set
###

    test perl.v8.base.ref {$SPARCEMU perl.v8.base.ref} {refInput} {
        set myStdout ""
        set myStderr ""
        set program "/proj/walkabout/spec95/benchspec/CINT95/134.perl/exe/base/perl.v8"
        set script  "/proj/walkabout/spec95/benchspec/CINT95/134.perl/data/test/input/primes.pl"
        set inFile  "/proj/walkabout/spec95/benchspec/CINT95/134.perl/data/ref/input/primes.in"
        set result [execCmd "$EMUPATH/$SPARCEMU $program $script < $inFile" myStdout myStderr]
        set outData [::tcltest::viewFile /proj/walkabout/spec95/benchspec/CINT95/134.perl/data/ref/output/primes.out]
        list $result [string equal $myStdout $outData] $myStderr
    } {0 1 {Interpreted 3681016 instructions}}


###
### perl.v8 peak optimization, test data set
###

    test perl.v8.peak.test {$SPARCEMU perl.v8.peak.test} {
        set myStdout ""
        set myStderr ""
        set program "/proj/walkabout/spec95/benchspec/CINT95/134.perl/exe/peak/perl.v8"
        set script  "/proj/walkabout/spec95/benchspec/CINT95/134.perl/data/test/input/primes.pl"
        set inFile  "/proj/walkabout/spec95/benchspec/CINT95/134.perl/data/test/input/primes.in"
        set result [execCmd "$EMUPATH/$SPARCEMU $program $script < $inFile" myStdout myStderr]
        set outData [::tcltest::viewFile /proj/walkabout/spec95/benchspec/CINT95/134.perl/data/test/output/primes.out]
        list $result [string equal $myStdout $outData] $myStderr
    } {0 1 {Interpreted 10365419 instructions}}


###
### perl.v8 peak optimization, reference data set
###

    test perl.v8.peak.ref {$SPARCEMU perl.v8.peak.ref} {refInput} {
        set myStdout ""
        set myStderr ""
        set program "/proj/walkabout/spec95/benchspec/CINT95/134.perl/exe/peak/perl.v8"
        set script  "/proj/walkabout/spec95/benchspec/CINT95/134.perl/data/test/input/primes.pl"
        set inFile  "/proj/walkabout/spec95/benchspec/CINT95/134.perl/data/ref/input/primes.in"
        set result [execCmd "$EMUPATH/$SPARCEMU $program $script < $inFile" myStdout myStderr]
        set outData [::tcltest::viewFile /proj/walkabout/spec95/benchspec/CINT95/134.perl/data/ref/output/primes.out]
        list $result [string equal $myStdout $outData] $myStderr
    } {0 1 {Interpreted 3681016 instructions}}


###
### go.v8 base optimization, test data set
###

    test go.v8.base.test {$SPARCEMU go.v8.base.test} {
        set myStdout ""
        set myStderr ""
        set program "/proj/walkabout/spec95/benchspec/CINT95/099.go/exe/base/go.v8"
        set params [::tcltest::viewFile /proj/walkabout/spec95/benchspec/CINT95/099.go/data/test/input/null.siz]
        set result [execCmd "$EMUPATH/$SPARCEMU $program $params" myStdout myStderr]
        set outData [::tcltest::viewFile /proj/walkabout/spec95/benchspec/CINT95/099.go/data/test/output/null.out]
        list $result [string equal $myStdout $outData] $myStderr
    } {0 1 {Interpreted 3681016 instructions}}


###
### go.v8 base optimization, reference data set
###

    test go.v8.base.ref {$SPARCEMU go.v8.base.ref} {refInput} {
        set myStdout ""
        set myStderr ""
        set program "/proj/walkabout/spec95/benchspec/CINT95/099.go/exe/base/go.v8"
        set params [::tcltest::viewFile /proj/walkabout/spec95/benchspec/CINT95/099.go/data/ref/input/null.siz]
        set result [execCmd "$EMUPATH/$SPARCEMU $program $params" myStdout myStderr]
        set outData [::tcltest::viewFile /proj/walkabout/spec95/benchspec/CINT95/099.go/data/ref/output/null.out]
        list $result [string equal $myStdout $outData] $myStderr
    } {0 1 {Interpreted 3681016 instructions}}


###
### go.v8 peak optimization, test data set
###

    test go.v8.peak.test {$SPARCEMU go.v8.peak.test} {
        set myStdout ""
        set myStderr ""
        set program "/proj/walkabout/spec95/benchspec/CINT95/099.go/exe/peak/go.v8"
        set params [::tcltest::viewFile /proj/walkabout/spec95/benchspec/CINT95/099.go/data/test/input/null.siz]
        set result [execCmd "$EMUPATH/$SPARCEMU $program $params" myStdout myStderr]
        set outData [::tcltest::viewFile /proj/walkabout/spec95/benchspec/CINT95/099.go/data/test/output/null.out] 
        list $result [string equal $myStdout $outData] $myStderr
    } {0 1 {Interpreted 3681016 instructions}}


###
### go.v8 peak optimization, reference data set
###

    test go.v8.peak.ref {$SPARCEMU go.v8.peak.ref} {refInput} {
        set myStdout ""
        set myStderr ""
        set program "/proj/walkabout/spec95/benchspec/CINT95/099.go/exe/peak/go.v8"
        set params [::tcltest::viewFile /proj/walkabout/spec95/benchspec/CINT95/099.go/data/ref/input/null.siz]
        set result [execCmd "$EMUPATH/$SPARCEMU $program $params" myStdout myStderr]
        set outData [::tcltest::viewFile /proj/walkabout/spec95/benchspec/CINT95/099.go/data/ref/output/null.out]
        list $result [string equal $myStdout $outData] $myStderr
    } {0 1 {Interpreted 3681016 instructions}}


###
### ijpeg.v8 base optimization, test data set
###

    test ijpeg.v8.base.test {$SPARCEMU ijpeg.v8.base.test} {
        set myStdout ""
        set myStderr ""
        set program "/proj/walkabout/spec95/benchspec/CINT95/132.ijpeg/exe/base/ijpeg.v8"
        set inFile  "/proj/walkabout/spec95/benchspec/CINT95/132.ijpeg/data/test/input/specmun.ppm"
        set opts {-compression.quality 90 -compression.optimize_coding 0 -compression.smoothing_factor 90 -difference.image 1 -difference.x_stride 10 -difference.y_stride 10 -verbose 1 -GO.findoptcomp }
        set result [execCmd "$EMUPATH/$SPARCEMU $program -image_file $inFile $opts" myStdout myStderr]
        set outData [::tcltest::viewFile /proj/walkabout/spec95/benchspec/CINT95/132.ijpeg/data/test/output/specmun.out]
        list $result [string equal $myStdout $outData] $myStderr
    } {0 1 {Interpreted 462386033 instructions}}


###
### ijpeg.v8 base optimization, reference data set
###

    test ijpeg.v8.base.ref {$SPARCEMU ijpeg.v8.base.ref} {refInput} {
        set myStdout ""
        set myStderr ""
        set program "/proj/walkabout/spec95/benchspec/CINT95/132.ijpeg/exe/base/ijpeg.v8"
        set inFile  "/proj/walkabout/spec95/benchspec/CINT95/132.ijpeg/data/ref/input/specmun.ppm"
        set opts {-compression.quality 90 -compression.optimize_coding 0 -compression.smoothing_factor 90 -difference.image 1 -difference.x_stride 10 -difference.y_stride 10 -verbose 1 -GO.findoptcomp }
        set result [execCmd "$EMUPATH/$SPARCEMU $program -image_file $inFile $opts" myStdout myStderr]
        set outData [::tcltest::viewFile /proj/walkabout/spec95/benchspec/CINT95/132.ijpeg/data/ref/output/specmun.out]
        list $result [string equal $myStdout $outData] $myStderr
    } {0 1 {Interpreted 3681016 instructions}}


###
### ijpeg.v8 peak optimization, test data set
###

    test ijpeg.v8.peak.test {$SPARCEMU ijpeg.v8.peak.test} {
        set myStdout ""
        set myStderr ""
        set program "/proj/walkabout/spec95/benchspec/CINT95/132.ijpeg/exe/peak/ijpeg.v8"
        set inFile  "/proj/walkabout/spec95/benchspec/CINT95/132.ijpeg/data/test/input/specmun.ppm"
        set opts {-compression.quality 90 -compression.optimize_coding 0 -compression.smoothing_factor 90 -difference.image 1 -difference.x_stride 10 -difference.y_stride 10 -verbose 1 -GO.findoptcomp }
        set result [execCmd "$EMUPATH/$SPARCEMU $program -image_file $inFile $opts" myStdout myStderr]
        set outData [::tcltest::viewFile /proj/walkabout/spec95/benchspec/CINT95/132.ijpeg/data/test/output/specmun.out] 
        list $result [string equal $myStdout $outData] $myStderr
    } {0 1 {Interpreted 462386033 instructions}}


###
### ijpeg.v8 peak optimization, reference data set
###

    test ijpeg.v8.peak.ref {$SPARCEMU ijpeg.v8.peak.ref} {refInput} {
        set myStdout ""
        set myStderr ""
        set program "/proj/walkabout/spec95/benchspec/CINT95/132.ijpeg/exe/peak/ijpeg.v8"
        set inFile  "/proj/walkabout/spec95/benchspec/CINT95/132.ijpeg/data/test/input/specmun.ppm"
        set opts {-compression.quality 90 -compression.optimize_coding 0 -compression.smoothing_factor 90 -difference.image 1 -difference.x_stride 10 -difference.y_stride 10 -verbose 1 -GO.findoptcomp }
        set result [execCmd "$EMUPATH/$SPARCEMU $program -image_file $inFile $opts" myStdout myStderr]
        set outData [::tcltest::viewFile /proj/walkabout/spec95/benchspec/CINT95/132.ijpeg/data/ref/output/specmun.out]
        list $result [string equal $myStdout $outData] $myStderr
    } {0 1 {Interpreted 3681016 instructions}}

}
### end of sparc tests



###
### Print statistics about tests run and clean up temporary files.
###

set finish [clock seconds]
puts "Tests finished [clock format $finish]."
puts "Tests required [expr {$finish - $start}] seconds total.\n"

::tcltest::cleanupTests

