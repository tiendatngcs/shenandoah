# To change to the newly build jdk:
1. Go to /etc/profile and add these lines:
    + JAVA_HOME=**path-to-jdk**
    + PATH=**$PATH:$HOME/bin:$JAVA_HOME/bin**
2. Run in shell:
    + **~/.profile**
3. Close the current and open a new shell.

# Upate alternative java path
/home/minh/Desktop/shenandoah/build/linux-x86_64-server-release/jdk/bin/javac
## Install
1. sudo update-alternatives --install /usr/bin/java java **path-to-jdk**/bin/java 1
2. sudo update-alternatives --install /usr/bin/javac javac **path-to-jdk**/bin/javac 1

## Change
1. sudo update-alternatives --config java
2. sudo update-alternatives --config javac

## Add this to your .bashrc to automatically update JAVA_HOME
```
#!/bin/bash
if [ -z "${JAVA_HOME}" ]
then
    JAVA_HOME=$(readlink -nf $(which java) | xargs dirname | xargs dirname | xargs dirname)
    if [ ! -e "$JAVA_HOME" ]
    then
        JAVA_HOME=""
    fi
    export JAVA_HOME=$JAVA_HOME
fi
```

# Enable ShenandoahGC
1. java **-XX:+UseShenandoahGC** filename 
    + Note: if you put the flag behind your filename, it will not work.

# Some useful commands
1. `jps`
    + check if id of a jvm
2. `sudo echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope`
    + enable jhsdb tracing
3. `jhsdb jmap --heap --pid <jvm id> | grep GC`
    + check the current GC running on the jvm

# Shenandoah GC flags
1. `-Xlog:gc`
    + Print the individual GC timing.
2. `-Xlog:gc+ergo`
    + Print the heuristics decisions, which might shed light on outliers, if any.
3. `-Xlog:gc+stats`
    + Print the summary table on Shenandoah internal timings at the end of the run. It is best to run this with logging enabled. This summary table conveys important information about GC performance. Heuristics logs are useful to figure out GC outliers.
4. `-XX:+AlwaysPreTouch`
    + Commit heap pages into memory and helps to reduce latency hiccups.
5. `-Xms` and `-Xmx`
    + Making the heap non-resizeable with `-Xms = -Xmx` reduces difficulties with heap management. Along with AlwaysPreTouch, the `-Xms = -Xmx` commit all memory on startup, which avoids difficulties when memory is finally used. `-Xms` also defines the low boundary for memory uncommit, so with `-Xms = -Xmx` all memory stays committed. If you want to configure Shenandoah for a lower footprint, then setting lower `-Xms` is recommended. You need to decide how low to set it to balance the commit/uncommit overhead versus memory footprint. In many cases, you can set `-Xms` arbitrarily low.
6. `-XX:+UseLargePages`
    + Enables hugetlbfs Linux support.
7. `-XX:+UseTransparentHugePages`
    + Enables huge pages transparently. With transparent huge pages, it is recommended to set /sys/kernel/mm/transparent_hugepage/enabled and /sys/kernel/mm/transparent_hugepage/defrag to madvise. When running with AlwaysPreTouch, it will also pay the defrag tool costs upfront at startup.
8. `-XX:+UseNUMA`
    + While Shenandoah does not support NUMA explicitly yet, it is a good idea to enable NUMA interleaving on multi-socket hosts. Coupled with AlwaysPreTouch, it provides better performance than the default out-of-the-box configuration.
9. `-XX:-UseBiasedLocking`
    + There is a tradeoff between uncontended (biased) locking throughput, and the safepoints JVM does to enable and disable them. For latency-oriented workloads, turn biased locking off.
10. `-XX:+DisableExplicitGC`
    + Invoking System.gc() from user code forces Shenandoah to perform additional GC cycle. It usuallydoes not harm, as `-XX:+ExplicitGCInvokesConcurrent` gets enabled by default, which means theconcurrent GC cycle would be invoked, not the STW Full GC.