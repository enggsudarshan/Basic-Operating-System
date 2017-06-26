0-README.txt

Demo.dli uses the original 1666667 times IO_DELAY(), 0.6 microseconds each.
Since VirtualBox runs much slower with this, VM-Demo.dli uses 166667 times
instead (1/10). Otherwise, it would be too slow to run.

Adjust the original loop number to get close to one-second delay if you use
VirtualBox or other VM-ware, in order to get a uniform timing through all
project phases.

Demo.dli is based on my code for Step 1; VM-Demo is from my code for Step 2,
the final product. Putting out all 4 variations will be confusing. Basicly
they behave the same when run. The PID showing of the same process is clearly
what TIME_SLICE is set in Step 1 but for Step 2, even running in the lab, the
actual runtime has the PID showing up among processes differ by one time since
the timer count is right at the multiple of hundreds (300). (Will changing this
to something like 320 fix it?)


