function switchTab(tab) {

    document.querySelectorAll('.tab-btn').forEach(btn => btn.classList.remove('active'));
    document.getElementById(`tab-${tab}`).classList.add('active');

    document.querySelectorAll('.view').forEach(view => view.classList.remove('active'));
    document.getElementById(`view-${tab}`).classList.add('active');
}

const pesos = {
    u1: 0.20,
    parcial: 0.25,
    u2: 0.20,
    final: 0.35
};
const NOTA_MINIMA = 10.5;

function toggleSubnotes(compId) {
    const isChecked = document.getElementById(`check-sub-${compId}`).checked;
    const single = document.getElementById(`single-${compId}`);
    const multi = document.getElementById(`multi-${compId}`);

    if (isChecked) {
        single.classList.remove('active');
        multi.classList.add('active');

        if (document.getElementById(`subnotes-list-${compId}`).children.length === 0) {
            addSubnote(compId);
        }
    } else {
        multi.classList.remove('active');
        single.classList.add('active');
    }
}

function addSubnote(compId) {
    const list = document.getElementById(`subnotes-list-${compId}`);
    const div = document.createElement('div');
    div.className = 'subnote-item';
    div.innerHTML = `
        <div class="input-suffix-wrapper">
            <input type="number" placeholder="Peso" min="0" max="100" class="sub-peso">
            <span class="input-suffix">%</span>
        </div>
        <input type="number" placeholder="Nota" min="0" max="20" class="sub-nota">
        <button class="btn-remove" onclick="this.parentElement.remove()" title="Eliminar"><i data-lucide="trash-2"></i></button>
    `;
    list.appendChild(div);
    lucide.createIcons();
}

const nombresComponentes = {
    u1: 'Unidad 1',
    parcial: 'Examen Parcial',
    u2: 'Unidad 2',
    final: 'Examen Final'
};

function markInvalid(input) {
    input.classList.add('input-error');
    input.addEventListener('input', function handler() {
        input.classList.remove('input-error');
        input.removeEventListener('input', handler);
    }, { once: true });
}

function clearAllErrors() {
    document.querySelectorAll('.input-error').forEach(el => el.classList.remove('input-error'));
}

function validateCourse() {
    clearAllErrors();
    let errors = [];
    let hasAnyData = false;
    const ids = ['u1', 'parcial', 'u2', 'final'];

    for (const compId of ids) {
        const toggle = document.getElementById(`check-sub-${compId}`);
        const hasSubnotes = toggle ? toggle.checked : false;
        const nombre = nombresComponentes[compId];

        if (hasSubnotes) {
            const subnotes = document.getElementById(`subnotes-list-${compId}`).children;

            if (subnotes.length === 0) {
                errors.push(`${nombre}: Activa sub-notas pero no hay ninguna añadida.`);
                continue;
            }


            for (let item of subnotes) {
                const pesoInput = item.querySelector('.sub-peso');
                const notaInput = item.querySelector('.sub-nota');
                const pesoVal = pesoInput.value.trim();
                const notaVal = notaInput.value.trim();

                // Si ambos estan vacios, saltar
                if (pesoVal === "" && notaVal === "") continue;

                const peso = parseFloat(pesoVal);
                const nota = parseFloat(notaVal);

                // Tiene nota pero no peso: error (necesitamos saber cuanto vale)
                if (notaVal !== "" && (pesoVal === "" || isNaN(peso))) {
                    markInvalid(pesoInput);
                    errors.push(`${nombre}: Una sub-nota tiene nota pero le falta el peso.`);
                }

                // Validar rango del peso si esta presente
                if (pesoVal !== "" && !isNaN(peso)) {
                    if (peso < 0 || peso > 100) {
                        markInvalid(pesoInput);
                        errors.push(`${nombre}: El peso debe estar entre 0 y 100.`);
                    }
                }

                // Validar rango de la nota si esta presente
                if (notaVal !== "" && !isNaN(nota)) {
                    hasAnyData = true;
                    if (nota < 0 || nota > 20) {
                        markInvalid(notaInput);
                        errors.push(`${nombre}: La nota debe estar entre 0 y 20.`);
                    }
                }

                // Peso con nota = dato completo
                if (pesoVal !== "" && notaVal !== "" && !isNaN(peso) && !isNaN(nota)) {
                    hasAnyData = true;
                }
            }

        } else {
            const input = document.getElementById(`nota-${compId}`);
            if (input && input.value.trim() !== "") {
                hasAnyData = true;
                const nota = parseFloat(input.value);
                if (isNaN(nota) || nota < 0 || nota > 20) {
                    markInvalid(input);
                    errors.push(`${nombre}: La nota debe estar entre 0 y 20.`);
                }
            }
        }
    }

    if (!hasAnyData) {
        errors.push("Ingresa al menos una nota para calcular.");
    }

    return errors;
}

function calculateComponent(compId) {
    let acumuladoInterno = 0;
    let porcentajeInternoCompletado = 0;

    const toggle = document.getElementById(`check-sub-${compId}`);
    const hasSubnotes = toggle ? toggle.checked : false;

    if (hasSubnotes) {
        const subnotes = document.getElementById(`subnotes-list-${compId}`).children;
        for (let item of subnotes) {
            const peso = parseFloat(item.querySelector('.sub-peso').value);
            const notaText = item.querySelector('.sub-nota').value;

            if (!isNaN(peso) && notaText.trim() !== "") {
                const nota = parseFloat(notaText);
                acumuladoInterno += nota * (peso / 100.0);
                porcentajeInternoCompletado += peso;
            }
        }
    } else {
        const input = document.getElementById(`nota-${compId}`);
        if (input && input.value.trim() !== "") {
            const nota = parseFloat(input.value);
            if (!isNaN(nota)) {
                acumuladoInterno = nota;
                porcentajeInternoCompletado = 100;
            }
        }
    }

    const pesoEnPromedio = pesos[compId];
    return {
        puntosGanados: acumuladoInterno * pesoEnPromedio,
        avanceGlobal: (porcentajeInternoCompletado / 100.0) * (pesoEnPromedio * 100.0)
    };
}

function calculateCourse() {
    // Validar antes de calcular
    const errors = validateCourse();
    if (errors.length > 0) {
        showToast(errors[0], "error");
        return;
    }

    let sumaPuntos = 0;
    let sumaProcentaje = 0;

    const ids = ['u1', 'parcial', 'u2', 'final'];
    ids.forEach(id => {
        const res = calculateComponent(id);
        sumaPuntos += res.puntosGanados;
        sumaProcentaje += res.avanceGlobal;
    });

    const cName = document.getElementById('courseName').value.trim();
    document.getElementById('res-course-title').innerText = cName ? `Resumen: ${cName}` : "Resumen del Curso";

    document.getElementById('result-curso').classList.remove('hidden');

    document.getElementById('res-porcentaje').innerText = `${sumaProcentaje.toFixed(1)}%`;
    const bar = document.getElementById('res-progress-fill');
    bar.style.width = '0%';
    setTimeout(() => {
        bar.style.width = `${sumaProcentaje}%`;
        bar.style.backgroundColor = sumaProcentaje >= 100 ? 'var(--success)' : 'var(--warning)';
    }, 100);

    document.getElementById('res-puntos').innerText = sumaPuntos.toFixed(2);

    const metasCont = document.getElementById('res-metas-container');
    const metasBody = document.getElementById('res-metas-body');
    const finalMsg = document.getElementById('res-final-message');

    metasBody.innerHTML = '';
    finalMsg.classList.remove('hidden', 'approved', 'failed');

    if (sumaProcentaje < 100) {
        metasCont.classList.remove('hidden');
        finalMsg.classList.add('hidden');

        const porcentajeRestante = 100 - sumaProcentaje;
        const factorRestante = porcentajeRestante / 100.0;
        const metas = [10.5, 12, 14, 15, 16, 17, 18, 19, 20];
        let mathPossible = false;

        metas.forEach(meta => {
            const puntosFaltantes = meta - sumaPuntos;
            const tr = document.createElement('tr');
            let content = `<td>${meta}</td>`;

            if (puntosFaltantes <= 0) {
                content += `<td class="status-ok">¡YA LA TIENES!</td>`;
                mathPossible = true;
            } else {
                const necesaria = puntosFaltantes / factorRestante;
                if (necesaria > 20) {
                    content += `<td class="status-bad">Imposible (>20)</td>`;
                } else {
                    content += `<td class="status-val">${necesaria.toFixed(2)}</td>`;
                    mathPossible = true;
                }
            }
            tr.innerHTML = content;
            metasBody.appendChild(tr);
        });

        if (mathPossible) {
            showToast("Calculado. Aún puedes aprobar.", "success");
        } else {
            showToast("Matemáticamente difícil. Revisa metas.", "error");
        }

    } else {

        metasCont.classList.add('hidden');
        finalMsg.classList.remove('hidden');

        if (sumaPuntos >= NOTA_MINIMA) {
            finalMsg.classList.add('approved');
            finalMsg.innerText = `¡APROBADO! Felicidades :D`;
            showToast(`Curso completado. ¡Aprobaste!`, "success");
        } else {
            finalMsg.classList.add('failed');
            finalMsg.innerText = `REPROBADO :c`;
            showToast(`Curso completado. No te alcanzó.`, "error");
        }
    }
}

function resetCourse() {
    // 1. Limpiar nombre del curso
    document.getElementById('courseName').value = '';

    // 2. Limpiar todos los inputs numéricos (notas de single y subnotas)
    const viewCurso = document.getElementById('view-curso');
    viewCurso.querySelectorAll('input[type="number"]').forEach(input => input.value = '');

    // 3. Resetear los switches de subnotas a OFF
    const ids = ['u1', 'u2'];
    ids.forEach(compId => {
        const checkbox = document.getElementById(`check-sub-${compId}`);
        if (checkbox && checkbox.checked) {
            checkbox.checked = false;
            toggleSubnotes(compId); // Esto volverá a ocultar la lista multi-input y mostrar single-input
        }
        // Vaciar la lista de subnotas para que empiece de cero la próxima vez
        document.getElementById(`subnotes-list-${compId}`).innerHTML = '';
    });

    // 4. Ocultar el panel de resultados completo
    document.getElementById('result-curso').classList.add('hidden');

    showToast('Calculadora de curso reiniciada', 'success');
}

function addPgaCourse() {
    const list = document.getElementById('pga-courses-list');
    const rowCount = list.children.length + 1;
    const div = document.createElement('div');
    div.className = 'pga-row';
    div.innerHTML = `
        <input type="text" placeholder="Nombre (Ej. Física 1)" class="pga-nombre">
        <input type="number" placeholder="Nota" min="0" max="20" class="pga-nota">
        <input type="number" placeholder="Créditos" min="1" class="pga-creditos">
        <button class="btn-remove" onclick="this.parentElement.remove()" title="Eliminar"><i data-lucide="trash-2"></i></button>
    `;
    list.appendChild(div);
    lucide.createIcons();
}

function calculatePga() {
    let sumaProductos = 0;
    let sumaCreditos = 0;

    const rows = document.getElementById('pga-courses-list').children;
    if (rows.length === 0) {
        showToast("Por favor añade al menos un curso.", "error");
        return;
    }

    for (let row of rows) {
        const nota = parseFloat(row.querySelector('.pga-nota').value);
        const creditos = parseInt(row.querySelector('.pga-creditos').value);

        if (!isNaN(nota) && !isNaN(creditos) && creditos > 0) {
            sumaProductos += (nota * creditos);
            sumaCreditos += creditos;
        }
    }

    if (sumaCreditos > 0) {
        const pga = sumaProductos / sumaCreditos;
        document.getElementById('result-pga').classList.remove('hidden');
        document.getElementById('res-pga-value').innerText = pga.toFixed(2);

        let msg = "";
        if (pga < 10.5) {
            msg = "¡Alerta Roja! Tu ciclo está en peligro. ¡Cuidado!";
        } else if (pga >= 10.5 && pga < 15) {
            msg = "Aprobado, pero tranquilo. Sigue esforzándote para subir ese promedio.";
        } else {
            msg = "¡Excelente promedio! Sigue así, estás volando alto.";
        }
        document.getElementById('pga-msg').innerText = msg;

        showToast(`Tu PGA ha sido calculado.`, "success");
    } else {
        showToast("No se ingresaron créditos válidos.", "error");
    }
}

function resetPga() {
    document.getElementById('result-pga').classList.add('hidden');

    document.getElementById('pga-courses-list').innerHTML = '';

    addPgaCourse();

    showToast('Calculadora de PGA reiniciada', 'success');
}

function showToast(message, type = "success") {
    const container = document.getElementById('toast-container');
    const toast = document.createElement('div');
    toast.className = `toast ${type}`;

    const icon = type === "success"
        ? '<i data-lucide="check-circle"></i>'
        : '<i data-lucide="alert-circle"></i>';

    toast.innerHTML = `${icon} <span>${message}</span>`;
    container.appendChild(toast);
    lucide.createIcons();

    setTimeout(() => {
        toast.style.animation = "fadeOut 0.4s ease forwards";
        setTimeout(() => toast.remove(), 400);
    }, 3500);
}

window.onload = () => {
    addPgaCourse();
}
