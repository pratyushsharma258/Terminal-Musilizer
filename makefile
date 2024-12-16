EXEC = terminamusilizer

$(EXEC): main.cpp
	g++ -o $@ $^

clean:
	del $(EXEC).exe