# Fixed-point numbers

## What are fixed-point numbers?

In essence, fixed-point numbers are a very simple representation of rational (non-integer) numbers.
The idea is that there is a fixed number of digits representing the fractional part of a number. The same goes for the integer part, so a fixed-point number can be: 12.75, where 12 is the integer part and 75 the fractional part. In this example, if there are always two digits before and after the point, we call that a fixed-point number, at least in base 10.

## Why use fixed-point numbers?

Fixed-point numbers are mostly useful in the embedded world, where some micro-controllers don't have a floating point unit (FPU), that is: they don't have the required hardware to perform floating-point operations.
On those micro-controllers, floating-point arithmetic is still possible, but it is very slow.

On the other hand, fixed-point numbers are very simple. Their simplicity is also the reason why they are pretty fast for basic arithmetic operations. They also allow to choose the position of the radix point, which can be very convenient.

## $Q$ notation

The $Q$ notation is often used to represent binary fixed-point numbers. Imagine that we have a 8-bit micro-controller and want to perform some non-integer computations. To that end, we need parameter that goes from $0$ to $10$, with a step of $0.1$.

To satisfy those conditions, we need the integer part to go from $0$ to, at least, $10$, and the fractional part to have a smallest value lower than 0.1, that is $\leq2^{-4}$, as $2^{-4} = 0.0625$.

The $Q$ notation comes in handy, in that it lets use represent such a number as: $UQi.f$, where $i$ is the integer part, and $f$ the fractional part of the number we wish to represent.
The $U$ prefix means that we are dealing with unsigned numbers.
To meet our example's requirements, we need to have a $UQ4.4$ number.

Of course, a $UQ4.4$ number is stored as a single byte.

## Range of fixed-point numbers

One of the most important properties of fixed-point numbers is their range. The minimum value of an unsigned fixed-point number is $0$.
However, the maximum value depends on both its integer part $i$, and its fractional part $f$.
The maximum value corresponding to the integer part is $2^i - 1$, to which we must add the maximum of the fractional part: $1 - 2^{-f}$.
Adding the two values leads to:
$$ 0 \leq UQi.f \leq 2^i - 2^f.$$
For instance, the maximum value of a $Q4.4$ number is $2^4 - 2^{-4} = 16 - \frac{1}{16} = 15.9375.$

Now, if we consider signed fixed-point numbers, the minimum value becomes $-2^{i - 1}$, which is the same as a regular signed integer.
The maximum value, is $2^{i - 1} - 2^{-f}$.
A signed number's most significant bit holds the sign, so a $Q3.4$ fixed-point number requires 8 bits of data[^1].
[^1]: This is the Texas Instruments notation.

We can see that, for a signed fixed-point, the following applies:
$$ -2^{i - 1} \leq Qi.f \leq 2^{i - 1} - 2^{-f}. $$
For instance, a $Q3.4$ number goes from $-2^2 = -4$ to $2^2 - 2^{-4} = 3.9375$.

## Precision of fixed-point numbers

The precision of a fixed-point solely defined by its number of fractional bits:
$$
\epsilon(Qi.f) = 2^{-f}.
$$

## Internal representation
