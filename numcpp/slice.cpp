namespace numcpp {
    slice::slice() {
        this->start = 0;
        this->stop = 0;
        this->step = 0;
    }

    slice::slice(size_t stop) {
        this->start = 0;
        this->stop = stop;
        this->step = 1;
    }

    slice::slice(size_t start, size_t stop, long step) {
        this->start = start;
        this->stop = stop;
        this->step = step;
    }
}
