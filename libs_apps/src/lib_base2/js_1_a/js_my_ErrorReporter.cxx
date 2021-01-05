#include "j_confg.h" // -DDEBUG=1 // fg spelling

#include "jsapi.h"
#include "e_print.h"

extern
void
my_ErrorReporter(JSContext *cx, const char *message, JSErrorReport *report)
{

	e_print("# ERROR %s\n",        message );
	e_print("# ERROR file '%s'\n", report->filename );
	e_print("# ERROR line %d\n",   report->lineno );
	e_print("# ERROR %s:%d:%s\n",
		report->filename,
		report->lineno,
		message
	);
	e_print("# TODO # write code for new jsapi that gets str16\n"),
	fflush(0);
	// const char16_t*  report_linebuf = report->linebuf();

	// if( report_linebuf)  fprintf(stderr, "# text %s\n", report_linebuf );
	// // if( report->tokenptr) fprintf(stderr, "# tail %s\n", report->tokenptr );
	// fprintf(stderr, "# text %s\n", report->uclinebuf );
	// fprintf(stderr, "# tail %s\n", report->uctokenptr );
	// fprintf(stderr, "

#ifdef BORROWED_FROM_JS_C
	FILE * gErrFile = stderr;
	bool reportWarnings = true;

    int i, j, k, n;
    char *prefix, *tmp;
    const char *ctmp;

    if (!report) {
        fprintf(gErrFile, "%s\n", message);
        return;
    }

    /* Conditionally ignore reported warnings. */
    if (JSREPORT_IS_WARNING(report->flags) && !reportWarnings)
        return;

    prefix = NULL;
    if (report->filename)
        prefix = JS_smprintf("%s:", report->filename);
    if (report->lineno) {
        tmp = prefix;
        prefix = JS_smprintf("%s%u: ", tmp ? tmp : "", report->lineno);
        JS_free(cx, tmp);
    }
    if (JSREPORT_IS_WARNING(report->flags)) {
        tmp = prefix;
        prefix = JS_smprintf("%s%swarning: ",
                             tmp ? tmp : "",
                             JSREPORT_IS_STRICT(report->flags) ? "strict " : "");
        JS_free(cx, tmp);
    }

    /* embedded newlines -- argh! */
    while ((ctmp = strchr(message, '\n')) != 0) {
        ctmp++;
        if (prefix)
            fputs(prefix, gErrFile);
        fwrite(message, 1, ctmp - message, gErrFile);
        message = ctmp;
    }

    /* If there were no filename or lineno, the prefix might be empty */
    if (prefix)
        fputs(prefix, gErrFile);
    fputs(message, gErrFile);

    if (!report->linebuf) {
        fputc('\n', gErrFile);
        goto out;
    }

    /* report->linebuf usually ends with a newline. */
    n = strlen(report->linebuf);
    fprintf(gErrFile, ":\n%s%s%s%s",
            prefix,
            report->linebuf,
            (n > 0 && report->linebuf[n-1] == '\n') ? "" : "\n",
            prefix);
    n = PTRDIFF(report->tokenptr, report->linebuf, char);
    for (i = j = 0; i < n; i++) {
        if (report->linebuf[i] == '\t') {
            for (k = (j + 8) & ~7; j < k; j++) {
                fputc('.', gErrFile);
            }
            continue;
        }
        fputc('.', gErrFile);
        j++;
    }
    fputs("^\n", gErrFile);
 out:
    if (!JSREPORT_IS_WARNING(report->flags))
        gExitCode = EXITCODE_RUNTIME_ERROR;
    JS_free(cx, prefix);
#endif
}
