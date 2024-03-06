Note that realsense sdk need to be <= 2.53!
if -> build from source: need to ./scripts/setup_udev_rules.sh

```
cmake ../ -DBUILD_EXAMPLES=true
sudo make uninstall && make clean && make && sudo make install
```