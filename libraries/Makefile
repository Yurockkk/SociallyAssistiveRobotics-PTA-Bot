TARGETS=robotis-op2.Makefile managers.Makefile python.Makefile

.PHONY: release debug profile clean

release debug profile clean: $(TARGETS)

managers.Makefile: robotis-op2.Makefile

python.Makefile: managers.Makefile

%.Makefile:
	+@echo "# make" $(MAKECMDGOALS) $(@:.Makefile=)
	+@make -s -C $(@:.Makefile=) $(MAKECMDGOALS)
