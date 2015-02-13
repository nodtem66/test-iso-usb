.PHONY: clean All

All:
	@echo "----------Building project:[ test_iso - Debug ]----------"
	@cd "test_iso" && $(MAKE) -f  "test_iso.mk"
clean:
	@echo "----------Cleaning project:[ test_iso - Debug ]----------"
	@cd "test_iso" && $(MAKE) -f  "test_iso.mk" clean
