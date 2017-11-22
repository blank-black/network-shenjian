SUB = `ls -d */`

subdirs:
	for dir in $(SUB); do \
$(MAKE) -C $$dir; \
done

clean:
	for dir in $(SUB); do \
$(MAKE) clean -C $$dir; \
done

