.PHONY: clean All

All:
	@echo ----------Building project:[ Tutorials - Debug ]----------
	@cd "../../Programming/Tutorial 0.3.7" && "$(MAKE)" -f "Tutorials.mk"
clean:
	@echo ----------Cleaning project:[ Tutorials - Debug ]----------
	@cd "../../Programming/Tutorial 0.3.7" && "$(MAKE)" -f "Tutorials.mk" clean
