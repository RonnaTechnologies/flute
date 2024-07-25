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
$$
\begin{equation}
0 \leq UQi.f \leq 2^i - 2^f.
\end{equation}
$$
For instance, the maximum value of a $Q4.4$ number is $2^4 - 2^{-4} = 16 - \frac{1}{16} = 15.9375.$

Now, if we consider signed fixed-point numbers, the minimum value becomes $-2^{i - 1}$, which is the same as a regular signed integer.
The maximum value, is $2^{i - 1} - 2^{-f}$.
A signed number's most significant bit holds the sign, so a $Q3.4$ fixed-point number requires 8 bits of data[^1].
[^1]: This is the Texas Instruments notation.

We can see that, for a signed fixed-point, the following applies:
$$
\begin{equation}
-2^{i - 1} \leq Qi.f \leq 2^{i - 1} - 2^{-f}.
\end{equation}
$$
For instance, a $Q3.4$ number goes from $-2^2 = -4$ to $2^2 - 2^{-4} = 3.9375$.

## Precision of fixed-point numbers

The precision of a fixed-point solely defined by its number of fractional bits:
$$
\begin{equation}
\epsilon(Qi.f) = 2^{-f}.
\end{equation}
$$

## Internal representation

The internal representation of a fixed-point number depends on the number of bits that are necessary to store it.
As an example, a $UQ4.4$ requires 8 bits, so a single byte can be used to represent the number.
Similarly, a $Q3.4$ number also requires a full byte to be represented, as one extra bit is required to store its sign.
So, basically, to represent a $UQi.f$, $i + f$ bits are required, and for a $Qi.f$, $i + f + 1$ bits are needed.

## From and to fixed-point

A fixed-point number is stored as an integer, but it needs to represent an integer and a fractional part.
We know that the fractional part's precision is $2^{-f}$, so it seems quite natural to represent a fixed-point number $UQi.f$ as:
$$
\begin{equation}
UQi.f = \frac{N}{2^{-f}} = N \times 2^f,
\end{equation}
$$
where $N$ is the numerator of the fractional representation of the fixed-point number.
For instance, a $Q3.4$ number that represents the value $3.25$ is represented by: $N = 3.25 = 3 + 0.25 = 3 + \frac{4}{2^4} = \frac{3 \times 2^4 + 4}{2^4}$. If we multiply this expression by $2^f = 2^4$, we get $N \times 2^4 = 3 \times 2^4 + 4$. In computer programming, multiplying by $2^4$ is the same as shifting 4 bits to the left, i.e.: $N \times 2^4 \equiv N \ll 4$, which leads to the following:
$$
\begin{equation}
    UQi.f = N \ll f.
\end{equation}
$$

### Converting from floating-point

Some constants, like $\pi$, need to be initialized as floating-point numbers, as this is the most intuitive representation. As constants are evaluated at compile-time (at least in C++), it would be very interesting to be able to convert a given constant from its floating-point representation to a fixed-point number.



### Converting to floating-point
