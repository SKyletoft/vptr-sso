run: Set.class
	java -ea -cp . Set

%.class: %.java
	javac -g $<

clean:
	-rm *.class

.PHONY: clean
