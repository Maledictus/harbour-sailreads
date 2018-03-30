TEMPLATE = subdirs

SUBDIRS = \
	liboauth \
    sailreads \
    $${NULL}

OTHER_FILES = \
	rpm/sailreads.changes.in \
	rpm/sailreads.changes.run.in \
    rpm/sailreads.yaml
