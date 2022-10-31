# keep breakpoints of function in shared libraries,
# which are not yet loaded, active (they are skipped in default configuration) 

set breakpoint pending on

# Breakpoints
break MyClass_new
break MyClass_init
break MyClass_dealloc
break MyClass_addOne
run

# TUI 
tui enable
layout src
