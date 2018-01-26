.PHONY: all utilities charge light clean

all: utilities light

utilities:
	@echo "Building................... mTPCUtilities"
	@cd ../utilities/make && $(MAKE) --no-print-directory TARGET=all
    
charge:
	@echo "Building................... mTPCChargeSystem"
	@cd ../charge/make && $(MAKE) --no-print-directory TARGET=all

light:
	@echo "Building................... mTPCLightSystem"
	@cd ../light/make && $(MAKE) --no-print-directory TARGET=all

clean:
	@echo "Cleaning................... mTPCUtilities"
	@cd ../utilities/make && $(MAKE) --no-print-directory clean
#	@echo "Cleaning................... mTPCChargeSystem"
#	@cd ../charge/make && $(MAKE) --no-print-directory clean
	@echo "Cleaning................... mTPCLightSystem"
	@cd ../light/make && $(MAKE) --no-print-directory clean
