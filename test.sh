STOP_AT=2000

mkdir test/
for i in Default Debug Release; do
  time build/$i/yinspire --stop-at ${STOP_AT} --record test/$i $@
done 

for i in Default Debug Release; do
  time wine build/$i.Win32/yinspire.exe --stop-at ${STOP_AT} --record test/$i.Win32 $@
done

echo "OK"
ruby tools/Compare_EventLists.rb test/Default test/Debug 
ruby tools/Compare_EventLists.rb test/Debug test/Release 
ruby tools/Compare_EventLists.rb test/Release test/Default.Win32
ruby tools/Compare_EventLists.rb test/Default.Win32 test/Debug.Win32
ruby tools/Compare_EventLists.rb test/Debug.Win32 test/Release.Win32 
