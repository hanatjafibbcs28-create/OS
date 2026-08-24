class H2O {
public:
    mutex m;
    condition_variable cv;
    int cnth=0;
    int cnto=0;
    H2O() {}

    void hydrogen(function<void()> releaseHydrogen) {
        unique_lock<mutex> l(m);
        cv.wait(l, [this]{return cnth<2;});
        cnth++;
        if(cnth==2 && cnto==1){
            cnth = 0;
            cnto = 0;
        }
        releaseHydrogen();
        cv.notify_all();
    }

    void oxygen(function<void()> releaseOxygen) {
        unique_lock<mutex> l(m);
        cv.wait(l, [this]{return cnto<1;});
        cnto++;
        if(cnth==2 && cnto==1){
            cnth = 0;
            cnto = 0;
        }
        releaseOxygen();
        cv.notify_all();
    }
};
