$$
\begin{align}
[\text{prog}] &\to [\text{stmt}]^* \\
[\text{stmt}] &\to 
\begin{cases}
    \text{exit}([\text{expr}]); \\
    \text{decl}\space[\text{modifiers}]\space[\text{type}]\space\text{ident} =
    [\text{expr}];
\end{cases} \\
[\text{expr}] &\to
\begin{cases}
    \text{literal} \\
    \text{ident}
\end{cases} \\
[\text{modifiers}] &\to
\begin{cases}
 \text{const}\quad|\quad\text{mutable}\quad|\quad\text{unsigned}\quad|\quad\text{static} \\
\end{cases} \\
[\text{type}] &\to 
\begin{cases}
    \text{int} \quad|\quad \text{float} \quad|\quad \text{char} \quad|\quad \text{bool} \quad|\quad \text{String} \\
    \text{i8} \quad|\quad \text{i16} \quad|\quad \text{i32} \quad|\quad \text{i64} \quad|\quad \text{i128} \quad|\quad \text{isize} \\
    \text{u8} \quad|\quad \text{u16} \quad|\quad \text{u32} \quad|\quad \text{u64} \quad|\quad \text{u128} \quad|\quad \text{usize} \\
    \text{f8} \quad|\quad \text{f16} \quad|\quad \text{f32} \quad|\quad \text{f64} \\
\end{cases}
\end{align}


$$
