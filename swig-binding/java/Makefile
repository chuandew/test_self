calculator : calculator.cc calculator.h calculator_wrap
	g++ -shared -fPIC -o libcalculator.so calculator.cc calculator_wrap.cxx -I/usr/lib/jvm/java-1.8.0-openjdk/include -I/usr/lib/jvm/java-1.8.0-openjdk/include/linux
calculator_wrap : calculator.i
	swig -java -c++ calculator.i
java_demo: TestCalculator.java
	javac TestCalculator.java
	java -Djava.library.path=. TestCalculator
clean :
	rm libcalculator.so calculator_wrap.cxx Cal*.java cal*.java *.class