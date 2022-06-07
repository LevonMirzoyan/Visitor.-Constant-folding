 В этой задаче вам необходимо реализовать сворачивание констант в
дереве (constant folding). Например, у нас есть выражение (точнее, дерево, описывающее это
выражение) abs(var * sqrt(32.0 - 16.0)), на выходе мы должны получить дерево для
следующего выражения abs(var * 4.0), т. е. подвыражение sqrt(32.0 - 16.0) было вычислено.
Для того, чтобы определить, что выражение (Expression) на самом деле является числом
(Number), используйте оператор dynamic_cast. Все промежуточные узлы дерева, которые вы создали, нужно освободить.

Примечание.

Как можно использовать оператор dynamic_cast? Например,
если у вас есть указатель Expression *, и вы хотите узнать, указывает ли этот указатель на
самом деле на объект Number, то сделать это можно так:
Expression *expression = parse(code);
Number *number = dynamic_cast <Number *>(expression);
if (number)
    std::cout << "It's a number" << std::endl; 
else 
    std::cout << "It is not a number" << std::endl; 
Если expression действительно указывает на объект Number (или на один из его наследников,
но в нашем примере их нет), то оператор dynamic_cast<Number *> вернет правильный
указатель. Если expression указывает не на класс Number, то будет возвращен нулевой
указатель. Т. е. если в переменной number хранится нулевой указатель, значит expression не
указывает на Number на самом деле.
Для правильной работы оператора dynamic_cast в нашем примере требуется, чтобы класс
Expression был полиморфным, т. е. в нем должна быть как минимум одна виртуальная
функция (например, деструктор). Стандарт определяет работу dynamic_cast и с не
полиморфными типами, но в этом случае никаких проверок типа времени исполнения
выполняться не будет - часто, это не то, что требуется.
Если передать оператору dynamic_cast нулевой указатель, то он просто вернет нулевой
указатель нужного типа, поэтому проверять указатель перед передачей в dynamic_cast не
нужно.
