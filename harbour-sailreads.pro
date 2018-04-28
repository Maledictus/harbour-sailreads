TEMPLATE = subdirs

SUBDIRS = \
    liboauth \
    sailreads \
    $${NULL}

OTHER_FILES = \
    rpm/harbour-sailreads.changes.in \
    rpm/harbour-sailreads.changes.run.in \
    rpm/harbour-sailreads.spec \
    rpm/harbour-sailreads.yaml \
    .qmake.conf

