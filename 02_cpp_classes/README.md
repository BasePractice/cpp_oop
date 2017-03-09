### Определение класса

### Операторы


| Основные	| Альтернативные |
|-----------|----------------|
|&&	        |and
|&=	        |and_eq
|&		    |bitand
|bitor	    |bitor
|~		    |compl
|!		    |not
|!=	        |not_eq
|oror	    |or
|or		    |or_eq
|^		    |xor
|^=	        |xor_eq
|{	        |<%
|}	        |%>
|[	        |<:
|]	        |:>
|#	        |%:
|##	        |%:%:

|Отсновной	|Триграф|
|-----------|-------|
|{	        |??<
|}	        |??>
|[	        |??(
|]	        |??)
|#	        |??=
|\	        |??/
|^	        |??'
|or	        |??!
|~	        |??-


```cpp
%:include <iostream>
 
int main(int argc, char *argv<::>) 
<%
    if (argc > 1 and argv<:1:> not_eq NULL) <%
        std::cout << "Hello, " << argv<:1:> << '\n';
    %>
%>
```
