#
# Copyright (C) 2001, Sun Microsystems, Inc
#
# See the file "LICENSE.TERMS" for information on usage and
# redistribution of this file, and for a DISCLAIMER OF ALL
# WARRANTIES.
#
#

#######################################################################
# FILE:     summarize-mt-tests.tcl
# OVERVIEW: Tcl script that collects stdout/err information from a previous
#           mt-tests.tcl run and writes it to the file "summary.txt" in
#           the specified mt-tests.tcl output directory. mt-tests.tcl is used
#           to run in parallel the tests for interpreters generated from the
#           Walkabout framework.
#######################################################################

# To run: 
#       tclsh summarize-mt-tests.tcl <directory>
#       where <directory> was created by a mt-tests.tcl run.
#
#  8 Oct 01 - Brian: created file.

###
### Usage: display usage information for this test script and exit
###

proc usage {} {
    puts "Usage: tclsh summarize-mt-tests.tcl <directory>"
    puts "       where <directory> was created by a mt-tests.tcl run."
    exit
}

###
### The program must be run in the dynamic/test subdirectory.
###

set dirList [file split [pwd]]
set lastDir [lindex $dirList end]
if {[string compare $lastDir "test"] != 0} {
    puts "Error: summarize-mt-tests.tcl can only be run in the directory <uqbt>/dynamic/test"
    exit
}

###
### There should be a single argument: the name of a directory created by a
### mt-tests.tcl run.
###

if {$argc != 1} {
    usage
}
set dirName $argv

puts "Summarizing parallel Walkabout test results into $dirName/summary.txt"

###
### Now read each file in the specified directory and append its output with
### an appropriate header onto $dirName/summary.txt.
###

set currPwd [pwd]
if [catch {cd $dirName} err] {
    puts stderr "Cannot cd to $dirName: $err"
    exit
}

if [catch {open summary.txt w} out] {
    puts stderr "Cannot open $dirName/summary.txt: $out"
    exit
}
puts $out "\tSummary of mt-tests.tcl run in directory $dirName"
puts $out " "
set fileNames [lsort [glob -nocomplain "*.out"]]
foreach f $fileNames {
    # append the contents of $f
    puts "appending file $f"
    if [catch {open $f r} in] {
        puts stderr "Cannot open $dirName/$f: $in"
    } else {
        puts $out "\t\t\tFile $f:"
        puts $out " "
        
        set data [read -nonewline $in]
        puts $out $data
        close $in
        
        puts $out " "
        puts $out "================================================================================"
        puts $out " "
    }
}
close $out

cd $currPwd
