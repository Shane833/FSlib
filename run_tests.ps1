$tests = Get-ChildItem "tests/*.exe" # Get all the generated executables

# run each of the executables
foreach($test in $tests){
    & $test # & represents the call operator
}
