__ecereProp_NamedStringsBox_Set_namedStrings(struct __ecereNameSpace__ecere__com__Instance * this, struct __ecereNameSpace__ecere__com__Instance * value)
{
struct NamedStringsBox * __ecerePointer_NamedStringsBox = (struct NamedStringsBox *)(this ? (((char *)this) + __ecereClass_NamedStringsBox->offset) : 0);
struct __ecereNameSpace__ecere__com__Instance * row;

__ecereMethod___ecereNameSpace__ecere__gui__controls__ListBox_Clear(__ecerePointer_NamedStringsBox->list);
if(value)
{
{
struct NamedString ** s;
struct __ecereNameSpace__ecere__com__Instance * __internalArray = (value);

for(s = (struct NamedString **)((struct __ecereNameSpace__ecere__com__Array *)(((char *)__internalArray + 24)))->array; s < (struct NamedString **)((struct __ecereNameSpace__ecere__com__Array *)(((char *)__internalArray + 24)))->array + ((struct __ecereNameSpace__ecere__com__Array *)(((char *)__internalArray + 24)))->count; s++)
{
char temp[797];

row = __ecereMethod___ecereNameSpace__ecere__gui__controls__ListBox_AddRow(__ecerePointer_NamedStringsBox->list);
__ecereMethod___ecereNameSpace__ecere__gui__controls__DataRow_SetData(row, __ecerePointer_NamedStringsBox->nameField, __ecereProp_NamedString_Get_name((*s)));
__ecereMethod___ecereNameSpace__ecere__gui__controls__DataRow_SetData(row, __ecerePointer_NamedStringsBox->stringField, __ecereProp_NamedString_Get_string((*s)));
}
}
}
row = __ecereMethod___ecereNameSpace__ecere__gui__controls__ListBox_AddRow(__ecerePointer_NamedStringsBox->list);
__ecereProp___ecereNameSpace__ecere__gui__controls__ListBox_Set_currentRow(__ecerePointer_NamedStringsBox->list, __ecereProp___ecereNameSpace__ecere__gui__controls__ListBox_Get_firstRow(__ecerePointer_NamedStringsBox->list));
__ecereProp___ecereNameSpace__ecere__gui__Window_Set_modifiedDocument(__ecerePointer_NamedStringsBox->list, 0x0);
__ecereNameSpace__ecere__com__eInstance_FireSelfWatchers(this, __ecereProp_NamedStringsBox_namedStrings), __ecereNameSpace__ecere__com__eInstance_FireSelfWatchers(this, __ecerePropM_NamedStringsBox_namedStrings);
}