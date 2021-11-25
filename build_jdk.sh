# change to defalult open jdk
echo Change Boot JDK to defalult Open JDK
sudo update-alternatives --config java
sudo update-alternatives --config javac

# build the jdk
bash configure
make images

# update new alternative path
echo Update new alternative path
cd build/linux-x86_64-server-release/jdk/
JDK=$(pwd)
sudo update-alternatives --install /usr/bin/java java $JDK/bin/java 1
sudo update-alternatives --install /usr/bin/javac javac $JDK/bin/javac 1
sudo update-alternatives --config java
sudo update-alternatives --config javac