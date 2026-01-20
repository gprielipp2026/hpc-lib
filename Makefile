# 1. Define your subdirectories
SUBDIRS := $(wildcard */)

# 2. Define the resulting libraries from those dirs
#    (Adjust names to match what your sub-makefiles actually produce)
SUBLIBS := $(foreach dir,$(SUBDIRS:%/=%),$(dir)/lib$(dir).a)

# 3. Define your final target
TARGET  := libhpc.a

.PHONY: all clean $(SUBDIRS)

all: $(TARGET)

# 4. Rule to build the sub-libraries
#    This triggers the Makefile inside each subdirectory
$(SUBDIRS):
	$(MAKE) -C $@

# 5. Ensure the sub-libs are built before the target
$(SUBLIBS): $(SUBDIRS)

# 6. The Merge Rule (Using an MRI Script)
#    We create a temporary script telling 'ar' to merge the libraries.
$(TARGET): $(SUBLIBS)
	@echo "Merging libraries into $@"
	@echo "create $@" > lib.mri
	@for lib in $(SUBLIBS); do \
		echo "addlib $$lib" >> lib.mri; \
	done
	@echo "save" >> lib.mri
	@echo "end" >> lib.mri
	ar -M < lib.mri
	@rm lib.mri

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
	rm -f $(TARGET)
