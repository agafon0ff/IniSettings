# IniSettings

Simple class to save and load ini-formatted file 

### Example load values:
```c++
IniSettings settings("config.ini");
settings.beginGroup("HELLO");
std::string  val1 = settings.value("val1");
int          val2 = settings.value("val2", 0);
double       val3 = settings.value("val3", 0.);
settings.endGroup();
```

### Example save values:
```c++
IniSettings settings("config.ini");
settings.beginGroup("HELLO");
settings.setValue("val1", "hello");
settings.setValue("val2", 123);
settings.setValue("val3", 1.23);
settings.endGroup();
```
