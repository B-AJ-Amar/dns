#ifndef REC_BALANCER_H
#define REC_BALANCER_H

const enum BALANCER_TYPE {
    RANDOM_BALANCER,
    FIRST_AVAILABLE_BALANCER,
    ROUND_ROBIN_BALANCER,
    ALWAYS_FIRST_BALANCER,

};

root_hint get_root_server(named_root_file *rnf, enum BALANCER_TYPE type, int *state);

#endif // REC_BALANCER_H