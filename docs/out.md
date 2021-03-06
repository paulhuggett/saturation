---
title: Saturation
---

<!-- <link rel="stylesheet" href="./foo.css"> -->
<script src="./foo.js"></script>

<fieldset>
    <legend>Columns</legend>
    <input type="checkbox" id="show-target"><label for="show-target">Show Target</label>
    <input type="checkbox" id="show-op"><label for="show-op">Show Op</label>
    <input type="checkbox" id="show-cpp"><label for="show-cpp">Show C++ Code</label>
    <input type="checkbox" id="show-asm" checked><label for="show-asm">Show Compiler Output</label>
</fieldset>
<label for="targets-select">Target:</label>
<select name="targets" id="targets-select">
<option value="cpu-x86_64">x86-64</option>
<option value="cpu-arm">ARM</option>
<option value="cpu-arm64">ARM64</option>
</select>
<label for="op-select">Operation:</label>
<select name="op" id="op-select">
<option value="op-add">add</option>
<option value="op-sub">subtract</option>
<option value="op-mul">multiply</option>
<option value="op-div">divide</option>
</select>

<table>
<thead>
<tr>
<th class="column-target">Target</th>
<th class="column-bits">Bits</th>
<th class="column-op">Op</th>
<th class="column-sign">Signed</th>
<th class="column-cpp">C++ Code</th>
<th class="column-asm">Compiler Output</th>
</tr>
</thead>
<tbody>

<tr class='sign-s op-add 4 cpu-x86_64'>
<td class='column-target'>x86_64</td>
<td class='column-bits'>4</td>
<td class='column-op'>add</td>
<td class='column-sign'>signed</td>
<td class='column-cpp'>
{% highlight cpp %}
#include "saturation.hpp"
using namespace saturation;
extern "C" sinteger_t<4>
f (sinteger_t<4> a,
   sinteger_t<4> b) {
    return adds<4>(a, b);
}
{% endhighlight %}
</td>
<td class='column-asm'>

{% highlight armasm %}
_f:                                     ## @f
	pushq	%rbp
	movq	%rsp, %rbp
	leal	(%rsi,%rdi), %eax
	addb	%dil, %dil
	andb	$16, %dil
	addb	$112, %dil
	sarb	$4, %dil
	movzbl	%dil, %ecx
	xorb	%sil, %dil
	xorb	%al, %sil
	notb	%sil
	orb	%dil, %sil
	shlb	$4, %al
	sarb	$4, %al
	testb	$8, %sil
	movzbl	%al, %eax
	cmovel	%ecx, %eax
	movsbl	%al, %eax
	popq	%rbp
	retq
{% endhighlight %}
</td>
</tr>
<tr class='sign-u op-add 4 cpu-x86_64'>
<td class='column-target'>x86_64</td>
<td class='column-bits'>4</td>
<td class='column-op'>add</td>
<td class='column-sign'>unsigned</td>
<td class='column-cpp'>
{% highlight cpp %}
#include "saturation.hpp"
using namespace saturation;
extern "C" uinteger_t<4>
f (uinteger_t<4> a,
   uinteger_t<4> b) {
    return addu<4>(a, b);
}
{% endhighlight %}
</td>
<td class='column-asm'>

{% highlight armasm %}
_f:                                     ## @f
	pushq	%rbp
	movq	%rsp, %rbp
	leal	(%rsi,%rdi), %eax
	movl	%eax, %ecx
	andb	$15, %cl
	cmpb	$16, %al
	movzbl	%cl, %eax
	movl	$15, %ecx
	cmovael	%ecx, %eax
	addb	%dil, %sil
	cmovbl	%ecx, %eax
	movzbl	%al, %eax
	popq	%rbp
	retq
{% endhighlight %}
</td>
</tr>
<tr class='sign-s op-sub 4 cpu-x86_64'>
<td class='column-target'>x86_64</td>
<td class='column-bits'>4</td>
<td class='column-op'>sub</td>
<td class='column-sign'>signed</td>
<td class='column-cpp'>
{% highlight cpp %}
#include "saturation.hpp"
using namespace saturation;
extern "C" sinteger_t<4>
f (sinteger_t<4> a,
   sinteger_t<4> b) {
    return subs<4>(a, b);
}
{% endhighlight %}
</td>
<td class='column-asm'>

{% highlight armasm %}
_f:                                     ## @f
	pushq	%rbp
	movq	%rsp, %rbp
	movl	%edi, %eax
	subb	%sil, %al
	xorb	%dil, %sil
	movzbl	%al, %ecx
	xorb	%dil, %al
	andb	%sil, %al
	shrb	$3, %dil
	andb	$1, %dil
	addb	$7, %dil
	testb	$8, %al
	movzbl	%dil, %eax
	cmovel	%ecx, %eax
	shlb	$4, %al
	sarb	$4, %al
	movsbl	%al, %eax
	popq	%rbp
	retq
{% endhighlight %}
</td>
</tr>
<tr class='sign-u op-sub 4 cpu-x86_64'>
<td class='column-target'>x86_64</td>
<td class='column-bits'>4</td>
<td class='column-op'>sub</td>
<td class='column-sign'>unsigned</td>
<td class='column-cpp'>
{% highlight cpp %}
#include "saturation.hpp"
using namespace saturation;
extern "C" uinteger_t<4>
f (uinteger_t<4> a,
   uinteger_t<4> b) {
    return subu<4>(a, b);
}
{% endhighlight %}
</td>
<td class='column-asm'>

{% highlight armasm %}
_f:                                     ## @f
	pushq	%rbp
	movq	%rsp, %rbp
	xorl	%ecx, %ecx
	subb	%sil, %dil
	movzbl	%dil, %eax
	cmovbl	%ecx, %eax
	andl	$15, %eax
	popq	%rbp
	retq
{% endhighlight %}
</td>
</tr>
<tr class='sign-s op-mul 4 cpu-x86_64'>
<td class='column-target'>x86_64</td>
<td class='column-bits'>4</td>
<td class='column-op'>mul</td>
<td class='column-sign'>signed</td>
<td class='column-cpp'>
{% highlight cpp %}
#include "saturation.hpp"
using namespace saturation;
extern "C" sinteger_t<4>
f (sinteger_t<4> a,
   sinteger_t<4> b) {
    return muls<4>(a, b);
}
{% endhighlight %}
</td>
<td class='column-asm'>

{% highlight armasm %}
_f:                                     ## @f
	pushq	%rbp
	movq	%rsp, %rbp
	movl	%esi, %eax
	mulb	%dil
	movl	%eax, %ecx
	sarb	$4, %cl
	movl	%eax, %edx
	sarb	$3, %dl
	cmpb	%dl, %cl
	je	LBB0_2
	xorb	%dil, %sil
	addb	%sil, %sil
	andb	$16, %sil
	addb	$112, %sil
	sarb	$4, %sil
	movl	%esi, %eax
LBB0_2:
	movsbl	%al, %eax
	popq	%rbp
	retq
{% endhighlight %}
</td>
</tr>
<tr class='sign-u op-mul 4 cpu-x86_64'>
<td class='column-target'>x86_64</td>
<td class='column-bits'>4</td>
<td class='column-op'>mul</td>
<td class='column-sign'>unsigned</td>
<td class='column-cpp'>
{% highlight cpp %}
#include "saturation.hpp"
using namespace saturation;
extern "C" uinteger_t<4>
f (uinteger_t<4> a,
   uinteger_t<4> b) {
    return mulu<4>(a, b);
}
{% endhighlight %}
</td>
<td class='column-asm'>

{% highlight armasm %}
_f:                                     ## @f
	pushq	%rbp
	movq	%rsp, %rbp
	imull	%esi, %edi
	cmpl	$15, %edi
	movl	$15, %eax
	cmovbl	%edi, %eax
	movzbl	%al, %eax
	popq	%rbp
	retq
{% endhighlight %}
</td>
</tr>
<tr class='sign-s op-div 4 cpu-x86_64'>
<td class='column-target'>x86_64</td>
<td class='column-bits'>4</td>
<td class='column-op'>div</td>
<td class='column-sign'>signed</td>
<td class='column-cpp'>
{% highlight cpp %}
#include "saturation.hpp"
using namespace saturation;
extern "C" sinteger_t<4>
f (sinteger_t<4> a,
   sinteger_t<4> b) {
    return divs<4>(a, b);
}
{% endhighlight %}
</td>
<td class='column-asm'>

{% highlight armasm %}
_f:                                     ## @f
	pushq	%rbp
	movq	%rsp, %rbp
	movl	%edi, %eax
	leal	1(%rsi), %ecx
	leal	8(%rax), %edx
	orb	%cl, %dl
	andb	$15, %dl
	cmpb	$1, %dl
	adcw	$0, %ax
	cwtd
	idivw	%si
	movsbl	%al, %eax
	popq	%rbp
	retq
{% endhighlight %}
</td>
</tr>
<tr class='sign-u op-div 4 cpu-x86_64'>
<td class='column-target'>x86_64</td>
<td class='column-bits'>4</td>
<td class='column-op'>div</td>
<td class='column-sign'>unsigned</td>
<td class='column-cpp'>
{% highlight cpp %}
#include "saturation.hpp"
using namespace saturation;
extern "C" uinteger_t<4>
f (uinteger_t<4> a,
   uinteger_t<4> b) {
    return divu<4>(a, b);
}
{% endhighlight %}
</td>
<td class='column-asm'>

{% highlight armasm %}
_f:                                     ## @f
	pushq	%rbp
	movq	%rsp, %rbp
	movzbl	%dil, %eax
	divb	%sil
	movzbl	%al, %eax
	popq	%rbp
	retq
{% endhighlight %}
</td>
</tr>
<tr class='sign-s op-add 4 cpu-arm'>
<td class='column-target'>arm</td>
<td class='column-bits'>4</td>
<td class='column-op'>add</td>
<td class='column-sign'>signed</td>
<td class='column-cpp'>
{% highlight cpp %}
#include "saturation.hpp"
using namespace saturation;
extern "C" sinteger_t<4>
f (sinteger_t<4> a,
   sinteger_t<4> b) {
    return adds<4>(a, b);
}
{% endhighlight %}
</td>
<td class='column-asm'>

{% highlight armasm %}
_f:
	mov	r2, #16
	and	r2, r2, r0, lsl #1
	add	r0, r1, r0
	add	r2, r2, #112
	lsl	r2, r2, #24
	eor	r3, r1, r2, asr #28
	eor	r1, r0, r1
	lsl	r0, r0, #28
	mvn	r1, r1
	asr	r0, r0, #28
	orr	r1, r3, r1
	tst	r1, #8
	asreq	r0, r2, #28
	bx	lr
{% endhighlight %}
</td>
</tr>
<tr class='sign-u op-add 4 cpu-arm'>
<td class='column-target'>arm</td>
<td class='column-bits'>4</td>
<td class='column-op'>add</td>
<td class='column-sign'>unsigned</td>
<td class='column-cpp'>
{% highlight cpp %}
#include "saturation.hpp"
using namespace saturation;
extern "C" uinteger_t<4>
f (uinteger_t<4> a,
   uinteger_t<4> b) {
    return addu<4>(a, b);
}
{% endhighlight %}
</td>
<td class='column-asm'>

{% highlight armasm %}
_f:
	add	r1, r1, r0
	and	r2, r1, #255
	and	r0, r1, #15
	cmp	r2, #15
	movhi	r0, #15
	cmp	r2, r1
	movne	r0, #15
	bx	lr
{% endhighlight %}
</td>
</tr>
<tr class='sign-s op-sub 4 cpu-arm'>
<td class='column-target'>arm</td>
<td class='column-bits'>4</td>
<td class='column-op'>sub</td>
<td class='column-sign'>signed</td>
<td class='column-cpp'>
{% highlight cpp %}
#include "saturation.hpp"
using namespace saturation;
extern "C" sinteger_t<4>
f (sinteger_t<4> a,
   sinteger_t<4> b) {
    return subs<4>(a, b);
}
{% endhighlight %}
</td>
<td class='column-asm'>

{% highlight armasm %}
_f:
	eor	r2, r1, r0
	sub	r1, r0, r1
	eor	r3, r1, r0
	and	r2, r2, r3
	mov	r3, #1
	and	r0, r3, r0, lsr #3
	tst	r2, #8
	addne	r1, r0, #7
	lsl	r0, r1, #28
	asr	r0, r0, #28
	bx	lr
{% endhighlight %}
</td>
</tr>
<tr class='sign-u op-sub 4 cpu-arm'>
<td class='column-target'>arm</td>
<td class='column-bits'>4</td>
<td class='column-op'>sub</td>
<td class='column-sign'>unsigned</td>
<td class='column-cpp'>
{% highlight cpp %}
#include "saturation.hpp"
using namespace saturation;
extern "C" uinteger_t<4>
f (uinteger_t<4> a,
   uinteger_t<4> b) {
    return subu<4>(a, b);
}
{% endhighlight %}
</td>
<td class='column-asm'>

{% highlight armasm %}
_f:
	subs	r0, r0, r1
	movlo	r0, #0
	and	r0, r0, #15
	bx	lr
{% endhighlight %}
</td>
</tr>
<tr class='sign-s op-mul 4 cpu-arm'>
<td class='column-target'>arm</td>
<td class='column-bits'>4</td>
<td class='column-op'>mul</td>
<td class='column-sign'>signed</td>
<td class='column-cpp'>
{% highlight cpp %}
#include "saturation.hpp"
using namespace saturation;
extern "C" sinteger_t<4>
f (sinteger_t<4> a,
   sinteger_t<4> b) {
    return muls<4>(a, b);
}
{% endhighlight %}
</td>
<td class='column-asm'>

{% highlight armasm %}
_f:
	mul	r2, r1, r0
	eor	r0, r1, r0
	mov	r1, #16
	and	r0, r1, r0, lsl #1
	lsl	r3, r2, #24
	add	r0, r0, #112
	asr	r12, r3, #28
	lsl	r0, r0, #24
	cmp	r12, r3, asr #27
	asr	r0, r0, #24
	lsrne	r2, r0, #4
	lsl	r0, r2, #24
	asr	r0, r0, #24
	bx	lr
{% endhighlight %}
</td>
</tr>
<tr class='sign-u op-mul 4 cpu-arm'>
<td class='column-target'>arm</td>
<td class='column-bits'>4</td>
<td class='column-op'>mul</td>
<td class='column-sign'>unsigned</td>
<td class='column-cpp'>
{% highlight cpp %}
#include "saturation.hpp"
using namespace saturation;
extern "C" uinteger_t<4>
f (uinteger_t<4> a,
   uinteger_t<4> b) {
    return mulu<4>(a, b);
}
{% endhighlight %}
</td>
<td class='column-asm'>

{% highlight armasm %}
_f:
	mul	r2, r1, r0
	cmp	r2, #15
	movhs	r2, #15
	and	r0, r2, #255
	bx	lr
{% endhighlight %}
</td>
</tr>
<tr class='sign-s op-div 4 cpu-arm'>
<td class='column-target'>arm</td>
<td class='column-bits'>4</td>
<td class='column-op'>div</td>
<td class='column-sign'>signed</td>
<td class='column-cpp'>
{% highlight cpp %}
#include "saturation.hpp"
using namespace saturation;
extern "C" sinteger_t<4>
f (sinteger_t<4> a,
   sinteger_t<4> b) {
    return divs<4>(a, b);
}
{% endhighlight %}
</td>
<td class='column-asm'>

{% highlight armasm %}
_f:
	push	{r7, lr}
	mov	r7, sp
	add	r2, r0, #8
	add	r3, r1, #1
	orr	r2, r3, r2
	tst	r2, #15
	addeq	r0, r0, #1
	bl	___divsi3
	lsl	r0, r0, #24
	asr	r0, r0, #24
	pop	{r7, lr}
	bx	lr
{% endhighlight %}
</td>
</tr>
<tr class='sign-u op-div 4 cpu-arm'>
<td class='column-target'>arm</td>
<td class='column-bits'>4</td>
<td class='column-op'>div</td>
<td class='column-sign'>unsigned</td>
<td class='column-cpp'>
{% highlight cpp %}
#include "saturation.hpp"
using namespace saturation;
extern "C" uinteger_t<4>
f (uinteger_t<4> a,
   uinteger_t<4> b) {
    return divu<4>(a, b);
}
{% endhighlight %}
</td>
<td class='column-asm'>

{% highlight armasm %}
_f:
	push	{r7, lr}
	mov	r7, sp
	bl	___udivsi3
	pop	{r7, lr}
	bx	lr
{% endhighlight %}
</td>
</tr>
<tr class='sign-s op-add 4 cpu-arm64'>
<td class='column-target'>arm64</td>
<td class='column-bits'>4</td>
<td class='column-op'>add</td>
<td class='column-sign'>signed</td>
<td class='column-cpp'>
{% highlight cpp %}
#include "saturation.hpp"
using namespace saturation;
extern "C" sinteger_t<4>
f (sinteger_t<4> a,
   sinteger_t<4> b) {
    return adds<4>(a, b);
}
{% endhighlight %}
</td>
<td class='column-asm'>

{% highlight armasm %}
_f:                                     ; @f
	lsl	w8, w0, #1
	and	w8, w8, #0x10
	add	w8, w8, #112
	sbfx	w8, w8, #4, #4
	eor	w9, w8, w1
	add	w10, w1, w0
	eor	w11, w10, w1
	orn	w9, w9, w11
	sbfx	w10, w10, #0, #4
	tst	w9, #0x8
	csel	w0, w8, w10, eq
	ret
{% endhighlight %}
</td>
</tr>
<tr class='sign-u op-add 4 cpu-arm64'>
<td class='column-target'>arm64</td>
<td class='column-bits'>4</td>
<td class='column-op'>add</td>
<td class='column-sign'>unsigned</td>
<td class='column-cpp'>
{% highlight cpp %}
#include "saturation.hpp"
using namespace saturation;
extern "C" uinteger_t<4>
f (uinteger_t<4> a,
   uinteger_t<4> b) {
    return addu<4>(a, b);
}
{% endhighlight %}
</td>
<td class='column-asm'>

{% highlight armasm %}
_f:                                     ; @f
	add	w8, w1, w0
	and	w9, w8, #0xff
	cmp	w9, #15
	ccmp	w9, w8, #0, ls
	and	w8, w8, #0xf
	mov	w9, #15
	csel	w0, w9, w8, ne
	ret
{% endhighlight %}
</td>
</tr>
<tr class='sign-s op-sub 4 cpu-arm64'>
<td class='column-target'>arm64</td>
<td class='column-bits'>4</td>
<td class='column-op'>sub</td>
<td class='column-sign'>signed</td>
<td class='column-cpp'>
{% highlight cpp %}
#include "saturation.hpp"
using namespace saturation;
extern "C" sinteger_t<4>
f (sinteger_t<4> a,
   sinteger_t<4> b) {
    return subs<4>(a, b);
}
{% endhighlight %}
</td>
<td class='column-asm'>

{% highlight armasm %}
_f:                                     ; @f
	eor	w8, w1, w0
	sub	w9, w0, w1
	eor	w10, w9, w0
	and	w8, w8, w10
	ubfx	w10, w0, #3, #1
	add	w10, w10, #7
	tst	w8, #0x8
	csel	w8, w9, w10, eq
	sbfx	w0, w8, #0, #4
	ret
{% endhighlight %}
</td>
</tr>
<tr class='sign-u op-sub 4 cpu-arm64'>
<td class='column-target'>arm64</td>
<td class='column-bits'>4</td>
<td class='column-op'>sub</td>
<td class='column-sign'>unsigned</td>
<td class='column-cpp'>
{% highlight cpp %}
#include "saturation.hpp"
using namespace saturation;
extern "C" uinteger_t<4>
f (uinteger_t<4> a,
   uinteger_t<4> b) {
    return subu<4>(a, b);
}
{% endhighlight %}
</td>
<td class='column-asm'>

{% highlight armasm %}
_f:                                     ; @f
	subs	w8, w0, w1
	csel	w8, wzr, w8, lo
	and	w0, w8, #0xf
	ret
{% endhighlight %}
</td>
</tr>
<tr class='sign-s op-mul 4 cpu-arm64'>
<td class='column-target'>arm64</td>
<td class='column-bits'>4</td>
<td class='column-op'>mul</td>
<td class='column-sign'>signed</td>
<td class='column-cpp'>
{% highlight cpp %}
#include "saturation.hpp"
using namespace saturation;
extern "C" sinteger_t<4>
f (sinteger_t<4> a,
   sinteger_t<4> b) {
    return muls<4>(a, b);
}
{% endhighlight %}
</td>
<td class='column-asm'>

{% highlight armasm %}
_f:                                     ; @f
	mul	w8, w1, w0
	sxtb	w9, w8
	sbfx	w10, w8, #4, #4
	cmp	w10, w9, asr #3
	eor	w9, w1, w0
	lsl	w9, w9, #1
	and	w9, w9, #0x10
	add	w9, w9, #112
	sbfx	w9, w9, #4, #4
	csel	w8, w8, w9, eq
	sxtb	w0, w8
	ret
{% endhighlight %}
</td>
</tr>
<tr class='sign-u op-mul 4 cpu-arm64'>
<td class='column-target'>arm64</td>
<td class='column-bits'>4</td>
<td class='column-op'>mul</td>
<td class='column-sign'>unsigned</td>
<td class='column-cpp'>
{% highlight cpp %}
#include "saturation.hpp"
using namespace saturation;
extern "C" uinteger_t<4>
f (uinteger_t<4> a,
   uinteger_t<4> b) {
    return mulu<4>(a, b);
}
{% endhighlight %}
</td>
<td class='column-asm'>

{% highlight armasm %}
_f:                                     ; @f
	mul	w8, w1, w0
	mov	w9, #15
	cmp	w8, #15
	csel	w8, w8, w9, lo
	and	w0, w8, #0xff
	ret
{% endhighlight %}
</td>
</tr>
<tr class='sign-s op-div 4 cpu-arm64'>
<td class='column-target'>arm64</td>
<td class='column-bits'>4</td>
<td class='column-op'>div</td>
<td class='column-sign'>signed</td>
<td class='column-cpp'>
{% highlight cpp %}
#include "saturation.hpp"
using namespace saturation;
extern "C" sinteger_t<4>
f (sinteger_t<4> a,
   sinteger_t<4> b) {
    return divs<4>(a, b);
}
{% endhighlight %}
</td>
<td class='column-asm'>

{% highlight armasm %}
_f:                                     ; @f
	add	w8, w1, #1
	add	w9, w0, #8
	orr	w8, w8, w9
	tst	w8, #0xf
	cinc	w8, w0, eq
	sdiv	w8, w8, w1
	sxtb	w0, w8
	ret
{% endhighlight %}
</td>
</tr>
<tr class='sign-u op-div 4 cpu-arm64'>
<td class='column-target'>arm64</td>
<td class='column-bits'>4</td>
<td class='column-op'>div</td>
<td class='column-sign'>unsigned</td>
<td class='column-cpp'>
{% highlight cpp %}
#include "saturation.hpp"
using namespace saturation;
extern "C" uinteger_t<4>
f (uinteger_t<4> a,
   uinteger_t<4> b) {
    return divu<4>(a, b);
}
{% endhighlight %}
</td>
<td class='column-asm'>

{% highlight armasm %}
_f:                                     ; @f
	udiv	w0, w0, w1
	ret
{% endhighlight %}
</td>
</tr>
</tbody>
</table>
