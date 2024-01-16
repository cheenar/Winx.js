let freeMemory = formatted(os.freemem() * (1 / 1024) * (1 / 1024) * (1 / 1024)) // in GiB
let totalMemory = formatted(os.totalmem() * (1 / 1024) * (1 / 1024) * (1 / 1024)) // in GiB

console.debug(`FreeMem: ${freeMemory} GiB`)
console.debug(`TotalMem: ${totalMemory} GiB`)
console.debug(`Ratio: ${formatted(freeMemory / totalMemory) * 100}%`)
