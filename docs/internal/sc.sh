# run endless scripal to find segfault
until scripal --unittest=interface; [ $? -eq 139 ]; do printf '.'; done
