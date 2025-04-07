#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

namespace Helper {
    void setSizePolicyExpanding(QWidget *w) {
        w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }

    void setSizePolicyFixed(QWidget *w) {
        w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    }
};

#endif // HELPERFUNCTIONS_H
