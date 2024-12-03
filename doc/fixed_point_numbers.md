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

Let, $F, a floating-point number, its fixed-point representation is given by:
$$
\begin{equation}
UQi.f = F \times 2^f.
\end{equation}
$$
For instance, let's say we need to perform some computations that involve $\pi$ on a 8-bit micro-controller.
The integer part of $\pi$ is $3$, so we only 2 bits to represent it, which leaves us 6 bits for the fractional part.
If we use a $UQ2.6$ number, our approximation of $\pi$ will have a precision of $2^{-6} = 0.015625$.
Assuming that is a good enough approximation, we get the following fixed-point internal representation of $\pi$: $\lfloor \pi \times 2^6 \rfloor = 201$.
Since $201 = 3 \ times 2^6 + 9$, the fixed-point number that we get is in fact: $3 + \frac{9}{2^6} \approx 3.1406$.
The same approximation with a $UQ4.12$ number gives an internal representation of $12,868$, which represents the number $3.1416$.
Again, same approximation with a $UQ12.20$ leads to $3,294,198$, which represents the number $\approx 3.141592$.

None of the above will ever be very close to $\pi$, but they can be very good approximations, and, as we'll see later, they will lead to very fast computations.

### Converting to floating-point

Let's consider the last example of the previous section.
How did we get the approximation of $3.141592$ from the internal representation $3,294,198$ of our $UQ12.20$ number?
The answer is quite simple: instead of multiplying by $2^{20}$, we divided by $2^{20}$.
This leads to:
$$
\begin{equation}
F = UQi.f \times 2^{-f}.
\end{equation}
$$
Let's verify this equation: $\frac{3,294,198}{2^{20}} = \frac{3,294,198}{1,048,576} \approx 3.141592$.

### Converting from and to an integer

The same formulas apply to integer, so equations $(6)$ and $(7)$ can be used to retrieve an integer $I$ directly by replacing $F$ by $I$.

## Fixed-point arithmetic

One of the main advantages of fixed-point numbers is their ability to perform fast arithmetic operations.
The internal representation of a fixed-point number being an integer, operations are almost as fast as integer operations.

### Addition and subtraction

Things are quite simple for addition and subtraction of fixed-point numbers, as long as they share the same integer and fractional parts.
Let's consider addition of two fixed-point numbers.
We would like to add a $Qi_1.f_1$ number to a $Qi_2.f_2$ number.
These numbers can be represented by $Qi_1.f_1 = N_1 \times 2^{f_1}$ and $Qi_2.f_2 = N_2 \times 2^{f_2}$.
If $f_1 = f_2 = f$, we get immediately:
$$
\begin{equation}
Qi_1.f + Qi_2.f = \left( N_1 + N_2 \right) \times 2^{f}.
\end{equation}
$$
Of course things get a little bit more complicated if $f_1 \neq f_2$.

Another simple case is the addition of a fixed-point number $Qi.f$ and an integer $I$:
$$
\begin{equation}
I + Qi.f = N_I \times 2^{f} + Qi.f = \left( N_I + N \right) \times 2^f.
\end{equation}
$$

The subtraction isn't more complex than the addition, as the sign is handled by the underlying integer operation.
Overflow can happen, as it can happen with integer addition and subtraction.

### Multiplication

Now, say we want to multiply $Qi_1.f_1$ and $Qi_2.f_2$: 
$$
\begin{equation}
Qi_1.f_1 \times Qi_2.f_2 = \left( N_1 \times 2^{f_1} \right) \times \left( N_2 \times 2^{f_2} \right) = N_1\,N_2 \times 2^{f_1 + f_2}.
\end{equation}
$$
Let's call $Qi_\times.f_\times$ the result of the previous multiplication.
It leads to:
$$
Qi_\times.f_\times = Qi_1.f_1 \times Qi_2.f_2,
$$
and so:
$$
N_\times 2^{f_\times} = N_1\,N_2 \times 2^{f_1 + f_2},
$$
thus, the internal representation of the multiplication's result is:
$$
\begin{equation}
N_\times = \left( N_1\,N_2 \times 2^{f_1 + f_2} \right) \times 2^{-f_\times}.
\end{equation}
$$
The value of $f_\times$ can be anything we want, but great care has to be taken, as $N_1\,N_2 \times 2^{f_1 + f_2}$ can lead to integer overflow.

As an example, imagine we want to compute the perimeter of a circle.*
The diameter of the circle is $d = 1.75$, so its perimeter is $d \times \pi$.
We use $UQ10.22$ for both numbers, which requires a 32-bit integer internally, so $N_d = 1.75 \times 2^{22} = 7,340,032$ and $N_\pi = \pi \times 2^{22} \approx 13,176,795$.
We get $N_{d \times \pi} = N_d N_{\pi} \times 2^{f_d + f_{\pi}} = 7,340,032 \times 13,176,795 = 96,718,089,617,408$.
This intermediate value is a very big number that overflows 32-bit integers by a lot, so it needs to be stored in a temporary 64-bit integer.

Now we want the result to be $UQ10.22$ as well, so $f_{d \times \pi} = 22$, which implies that the internal representation of the multiplication is $96,718,089,617,408 \times 2^{-22} = 23,059,389$.
Let's convert that number back to a floating-point: $F_{d \times \pi} = 23,059,389 \times 2^{-22} \approx 5.49778688$.
The actual result is $5.49778714$, so that's not too bad of an approximation.

What we have done is multiply two 32-bit integers, store the result in a 64-bit integer and multiply by $2^{-22}$ to get the internal representation of the result.
Multiplying by $2^{-22}$ is the same as shifting right by 22 bits ($\gg 22$), which is computationally inexpensive. 

Multiplying by an integer $I$ is a little bit easier:
$$
\begin{equation}
Qi_\times.f_\times = I \times Qi.f.
\end{equation}
$$

### Reciprocal

So far, everything has been relatively straightforward, however, division is a bit more complex.
Dividing numbers is not easy, and the complexity of division leads to long computations.
In practice, division can be seen as the multiplication of one number (numerator), by the inverse of another (denominator).
A $UQi.f$ number ranges from $0$ to $2^i$, so the precision of $\frac{1}{UQi.f}$ is $2^{-i}$.
In other terms:
$$
\begin{equation}
\frac{1}{UQi.f} = UQf.x,
\end{equation}
$$
where $x$ is to be determined.
In practice, $x$ can be anything.
It can be a value that is specified by some requirements, or some value that is restricted by the internal storage, etc.
If a default value had to be chosen, $x = i$ would be a decent choice, as it wouldn't change the internal representation:
$$
\begin{equation}
\frac{1}{UQi.f} = UQf.i.
\end{equation}
$$

Let's see what happens if we compute the inverse of $\sqrt{2} \approx 1.414213562$.
For this example, we choose to use $UQ16.16$ numbers, which lead to the internal representation of $92,682$ for $\sqrt{2}$.
We need to divide something by a number whose value lies between $0$ and $2^{16}$.
The result of this division also needs lie between $0$ and $2^{16}$, so the numerator of our division has to be $2^{32}$.
From that, we get the internal representation of $\frac{2^{32}}{92,682} \approx 46,340$ (rounding down).
This number is also expressed in a $UQ16.16$ format, its corresponding floating-point value is: $46,340 / 2^{16} \approx 0.70709$. That is about the value we expected, as $\frac{1}{\sqrt{2}} \approx 0.707107$.
Note that rounding the internal representation to $46,341$ would have given use a better result.

This example is somewhat misleading, as we know that $\frac{1}{\sqrt{2}} = \frac{\sqrt{2}}{2}$.
Thus, the problem is a lot simpler than it looks, we just need to divide the internal representation of $\sqrt{2}$ by $2$, which leads to the following representation for $\frac{1}{\sqrt{2}}$: $92,682 \ll 1 = 46,341$.
This illustrates that we don't actually need division in this particular case.
The takeaway is: try to avoid division at all cost, as there might be better alternatives in some cases.

Of course, $\frac{2^{32}}{92,682}$ is an integer division, some micro-controllers do have instructions for that (mostly 32-bit micro-controllers), but these instructions usually take more than a single clock cycle, so they are more expensive than they look. On the other hand, this is to be avoided at all cost if the micro-controller doesn't have an instruction for division.


