import os
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
sns.set(rc = {'figure.figsize':(16,10)})

plots_folder = './graphs'

def print_graph(df: pd.DataFrame,
                x_axis: str,
                title: str,
                file_name: str):
    ax = sns.barplot(x=df[x_axis], y=df.total_cycles, palette='pastel')
    ax.set_title(f'{title} (cycles)')
    ax.set(xlabel=x_axis, ylabel='total cycles')
    ax.set_xticklabels(ax.get_xticklabels(), rotation=40, ha="right")
    ax.set_yscale("log")
    plt.tight_layout()
    plt.savefig(os.path.join(plots_folder, f'{file_name}_cycles.jpg'))
    plt.clf()

if __name__ == '__main__':
    df = pd.read_csv('performances.csv')

    # remove spaces
    df.columns = df.columns.str.replace(' ', '_')

    # observe general structure of the df
    print('Data frame:')
    print(df.head())

    print('Data frame infos:')
    print(df.info())

    # gather informations of the algorithms runnning on the whole 2048 samples with 116 bit filter
    whole_code_df = df[df['code_portion'] == 'whole code']
    print('Full code performances:')
    print(whole_code_df)

    print_graph(df=whole_code_df,
                x_axis='algorithm',
                title='Full code execution',
                file_name='full_code')

    nlms_df = df[df['algorithm'] == 'nlms']
    rls_df = df[df['algorithm'] == 'rls']
    block_nlms_df = df[df['algorithm'] == 'block_nlms']
    block_rls_df = df[df['algorithm'] == 'block_rls']

    # first row is the full code, so we don't want it
    print('NLMS performances:')
    print(nlms_df)
    print_graph(df=nlms_df.iloc[1:, :],
                x_axis='code_portion',
                title='NLMS Fine-grained Performances',
                file_name='nlms_1_it_code_perf')

    # first row is the full code, so we don't want it
    print('RLS performances:')
    print(rls_df)
    print_graph(df=rls_df.iloc[1:, :],
                x_axis='code_portion',
                title='RLS Fine-grained Performances',
                file_name='rls_1_it_code_perf')

    # first row is full code, so we don't want it
    print('Block NLMS performances:')
    print(block_nlms_df)
    print_graph(df=block_nlms_df.iloc[1:, :],
                x_axis='code_portion',
                title='Block NLMS Fine-grained Performances',
                file_name='block_nlms_1_it_code_perf')

    # first row is full code, so we don't want it
    print('Block RLS performances:')
    print(block_rls_df)
    print_graph(df=block_rls_df.iloc[1:, :],
                x_axis='code_portion',
                title='Block RLS Fine-grained Performances',
                file_name='block_rls_1_it_code_perf')