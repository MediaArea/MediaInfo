# Count messages

Count messages use the selected catalog's plural-rule metadata, independently
of the operating system locale. Older catalogs use the library's compatibility
rules selected by `  Language_ISO639`.
They are optional additions to the existing semicolon-separated CSV catalog.

Supported key families:

- `FileCount.<category>`: file count in the window title.
- `StreamSummary.<kind>.<category>`: count and format list in Easy view.
- `StreamSummaryMore.<kind>.<category>`: count and navigation text in Sheet view.
- `StreamCount.<kind>.<category>`: count and stream noun only.

Kinds are `Video`, `Audio`, `Text`, `Image`, `Other`, and `Menu`.
Categories are `zero`, `one`, `two`, `few`, `many`, and `other`.
Always provide `other`; missing categories use that message.
Only add categories needed by the language. These GUI counts are integers.

Each pattern must contain `{count}` exactly once. `StreamSummary` also requires
`{formats}` exactly once; the other families do not accept this argument.
Translators control word order and punctuation. Write `{{` and `}}` for literal
braces. Other argument names and unmatched braces are rejected. Standard CSV
quoting applies to semicolons, quotes, and line breaks. Inserted metadata is
literal text and is never interpreted as another pattern.

```text
StreamSummary.Audio.one;{count} audio stream: {formats}
StreamSummary.Audio.other;{count} audio streams: {formats}
FileCount.other;{count} files
```

Existing `stream1/2/3` and `file1/2/3` entries remain supported through a
language-specific compatibility mapping. Do not remove them. Invalid or missing
messages fall back to usable legacy translations, then to English with English
rules. Older MediaInfoLib versions without `Language_Format` use complete
English count messages; the locale-aware behavior requires the matching library.

Catalog migrations are separate localization changes. The source fix uses
existing count forms; missing families use the complete English fallback.

## Plural-rule metadata

Rules and legacy-form mappings belong to the catalog. Keep metadata synchronized
between `Source/Resource/Language.csv` and the individual catalog files.
These entries are configuration data; do not translate their keys or expressions.

```text
  Config_Text_PluralRules;1
  Config_Text_PluralRule.one;v == 0 && i == 1
  Config_Text_PluralLegacy;one=1,other=2
  Config_Text_PluralLegacyDecimals;1
```

The example selects the English singular only for integer `1`. Operands are
`i` (integer part), `v` (number of visible fractional digits), and `f` (fractional
digits as an integer). Predicates may use `%`, comparisons, `&&`, `||` and
parentheses. Categories are tested in order: `zero`, `one`, `two`, `few`, `many`;
`other` is the default. Empty predicates disable a category.

The optional legacy mapping assigns categories to existing `1/2/3` entries;
`other` supplies unspecified categories and `0` disables a legacy form.
The decimal setting permits (`1`) or disables (`0`, the default) numbered forms
for fractional notation. It does not restrict named category messages.
See MediaInfoLib's `Source/Doc/CountLocalization.md` for the complete grammar,
validation limits and fallback behavior. Missing or invalid metadata preserves
the library's existing language rules; metadata never borrows English rules
from a merged catalog.
