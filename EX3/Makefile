.PHONY: all clean algorithm gamemanager simulator externalmanager run_external

all: algorithm gamemanager simulator externalmanager

algorithm:
	$(MAKE) -C Algorithm

gamemanager:
	$(MAKE) -C GameManager

simulator:
	$(MAKE) -C Simulator

externalmanager:
	g++ -std=c++20 -Wall -Wextra -Werror -pedantic ExternalManager.cpp -o ExternalManager

run_external: externalmanager simulator gamemanager algorithm
	./ExternalManager \
		"Simulator/src/main" \
		"-competition" \
		"game_maps_folder=run/maps/" \
		"game_manager=GameManager/GameManager_209399021_208239152.so" \
		"algorithms_folder=run/algos/" \
		"num_threads=3" \
		"-verbose"

clean:
	$(MAKE) -C Algorithm clean
	$(MAKE) -C GameManager clean
	$(MAKE) -C Simulator clean
	rm -f ExternalManager
