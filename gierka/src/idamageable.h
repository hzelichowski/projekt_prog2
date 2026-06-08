#ifndef IDAMAGEABLE_H
#define IDAMAGEABLE_H

//7 Pojęcie interfejsu
class IDamageable {
public:
    virtual ~IDamageable() = default;

    virtual void take_damage(int amount) = 0;
    virtual void die() = 0;
};

#endif