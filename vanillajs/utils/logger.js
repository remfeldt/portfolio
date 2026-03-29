const APP_LOG_PREFIX = "[portfolio-vanillajs]";

/**
 * Logs informational messages with an optional context payload.
 * @param {string} message - The log message.
 * @param {any} [context] - Optional structured context data.
 */
export const logInfo = (message, context) => {
    if (context !== undefined) {
        console.info(`${APP_LOG_PREFIX} ${message}`, context);
        return;
    }

    console.info(`${APP_LOG_PREFIX} ${message}`);
};

/**
 * Logs warning messages with an optional context payload.
 * @param {string} message - The warning message.
 * @param {any} [context] - Optional structured context data.
 */
export const logWarning = (message, context) => {
    if (context !== undefined) {
        console.warn(`${APP_LOG_PREFIX} ${message}`, context);
        return;
    }

    console.warn(`${APP_LOG_PREFIX} ${message}`);
};

/**
 * Logs error messages with an optional error object or context payload.
 * @param {string} message - The error message.
 * @param {any} [errorOrContext] - Optional error instance or structured context data.
 */
export const logError = (message, errorOrContext) => {
    if (errorOrContext !== undefined) {
        console.error(`${APP_LOG_PREFIX} ${message}`, errorOrContext);
        return;
    }

    console.error(`${APP_LOG_PREFIX} ${message}`);
};
